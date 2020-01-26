/*
LuaXML License

LuaXML is licensed under the terms of the MIT license reproduced below,
the same as Lua itself. This means that LuaXML is free software and can be
used for both academic and commercial purposes at absolutely no cost.

Copyright (C) 2007-2013 Gerald Franz, eludi.net

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/// @module LuaXML

#include "LuaXML_lib.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* compatibility with older Lua versions (<5.2) */
#if LUA_VERSION_NUM < 502

// Substitute lua_objlen() for lua_rawlen()
#define lua_rawlen(L, index) lua_objlen(L, index)

// Make use of luaL_register() to achieve same result as luaL_newlib()
#define luaL_newlib(L, funcs)                                                  \
	do {                                                                       \
		lua_newtable(L);                                                       \
		luaL_register(L, NULL, funcs);                                         \
	} while (0)

#endif
/* API changes for 5.2+ */
#if LUA_VERSION_NUM >= 502

// lua_compare() has replaced lua_equal()
#define lua_equal(L, index1, index2) lua_compare(L, index1, index2, LUA_OPEQ)

#endif
/* API changes for 5.3+ */
#if LUA_VERSION_NUM >= 503

// luaL_optinteger() has replaced luaL_optint()
#define luaL_optint(L, arg, d) luaL_optinteger(L, arg, d)

#endif


#define LUAXML_META "LuaXML" // name to be used for metatable

//--- auxliary functions -------------------------------------------

static size_t
find(const char *s, const char *pattern, size_t start)
{
	const char *found = strstr(s + start, pattern);
	return found ? (size_t)(found - s) : strlen(s);
}

// push (arbitrary Lua) value to be used as tag key, placing it on top of stack
static inline void
push_TAG_key(lua_State *L)
{
	/* Note: Currently this is the number 0, which fits in nicely with using
	 * string keys for attribute-value pairs and also 'stays clear' of the
	 * array of sub-elements (starting at index 1).
	 * Theoretically, this could be any kind of Lua value; but when using a
	 * string key (e.g. "TAG"), extra care needs to be taken that it doesn't
	 * get confused with an attribute - which means that the str() function
	 * should be modified accordingly (to recognise and avoid the tag key).
	 */
	lua_pushinteger(L, 0);
}

// convert Lua table at given index to an XML "object", by setting its metatable
static void
make_xml_object(lua_State *L, int index)
{
	if (index < 0)
		index += lua_gettop(L) + 1; // relative to absolute index
	if (!lua_istable(L, index))
		luaL_error(L,
		           "%s() error: invalid type at %d - expected table, got %s",
		           __func__,
		           index,
		           luaL_typename(L, index));

	luaL_getmetatable(L, LUAXML_META);
	lua_setmetatable(L, index); // assign metatable
}

// push an indentation string for the given level to the Lua stack
static void
push_indentStr(lua_State *L, int level)
{
	if (level <= 0) {
		lua_pushliteral(L, "");
		return;
	}
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	// while (level-- > 0) luaL_addlstring(&b, "  ", 2);
	while (level-- > 0)
		luaL_addchar(&b, '\t'); // one TAB char per level
	luaL_pushresult(&b);
}

// tests if a string consists entirely of whitespace
static bool
is_whitespace(const char *s)
{
	if (!s)
		return false; // NULL pointer
	if (*s == 0)
		return false; // empty string
	while (*s)
		if (!isspace(*s++))
			return false;
	return true;
}

// We consider a token "lead in", if it 1) is all whitespace and 2) starts with
// a newline. (This is typical for line breaks plus indentation on nested XML.)
static bool
is_lead_token(const char *s)
{
	return is_whitespace(s) && (*s == '\n' || *s == '\r');
}

/*
 * For the string at given stack index, substitute any occurrence (exact string
 * match) of pattern "p" with the replacement string "r".
 * When done, this function will replace the original string with the result.
 */
// TODO / Caveat:
// We return the luaL_gsub() pointer, but it's unclear (and untested) if that
// persists after the lua_replace(). Currently the result isn't used anywhere.
static const char *
do_gsub(lua_State *L, int index, const char *p, const char *r)
{
	if (index < 0)
		index += lua_gettop(L) + 1; // relative to absolute index
	const char *result = luaL_gsub(L, lua_tostring(L, index), p, r);
	lua_replace(L, index);
	return result;
}

/*
 * Lua C function to replace a gsub() match with the corresponding character.
 * Xml_pushDecode() will use this as a replacement function argument to undo
 * the XML encodings, passing one match (sequence of digits) at a time.
 *
 * Due to the pattern used, the matched string may also be 'x' followed by
 * a sequence of hexadecimal characters ("xE4"), which is supported too.
 */
static int
XMLencoding_replacement(lua_State *L)
{
	const char *matched = lua_tostring(L, 1);
	if (matched) {
		// support both decimal and hexadecimal conversion
		char c = *matched == 'x' ? strtol(++matched, NULL, 16) : atoi(matched);
		if (c) {
			lua_pushlstring(L, &c, 1); // return character as Lua string
			return 1;
		} // c == 0 probably indicates conversion failure, return `nil`
	}
	return 0;
}

/* Lua C callback function for a `find()` match. Sets the upvalue (that will
 * later be the result) and stops the iteration.
 *
 * A small problem here is that the callback handling by iterate() means this
 * function cannot simply return the result on the Lua stack. Instead we need
 * a "shared" upvalue that can be retrieved 'externally' later. Therefore a
 * simple, 'flat' Lua value won't do (it can't be shared); so we'll use a table
 * instead and assign the match to t[1].
 */
static int
find_on_match(lua_State *L)
{
	// Upon entry the Lua stack will have `var` and `depth`
	lua_settop(L, 1); // discard depth, leaving var on the stack
	lua_rawseti(L, lua_upvalueindex(1), 1); // store to upvalue table
	lua_pushboolean(L, false);              // return false to stop iteration
	return 1;
}

/// strip all leading / trailing whitespace
//  @field WS_TRIM

/// remove "lead in" whitespace before tags
//  @field WS_NORMALIZE

/// preserve all whitespace, even between tags
//  @field WS_PRESERVE

enum whitespace_mode {
	WHITESPACE_TRIM,
	WHITESPACE_NORMALIZE,
	WHITESPACE_PRESERVE
};

// control chars used by the Tokenizer to denote special meanings
#define ESC 27 /* end of scope, closing tag */
#define OPN 28 /* "open", start of tag */
#define CLS 29 /* closes opening tag, actual content follows */

//--- internal tokenizer -------------------------------------------

typedef struct Tokenizer_s {
	/// stores string to be tokenized
	const char *s;
	/// stores size of string to be tokenized
	size_t s_size;
	/// stores current read position
	size_t i;
	/// stores current read context
	int tagMode;
	/// stores flag for "raw" byte sequence, *DON'T* decode any further
	int cdata;
	/// stores next token, if already determined
	const char *m_next;
	/// size of next token
	size_t m_next_size;
	/// pointer to current token
	char *m_token;
	/// size of current token
	size_t m_token_size;
	/// capacity of current token
	size_t m_token_capacity;
	/// whitespace handling
	enum whitespace_mode mode;
} Tokenizer;

Tokenizer *
Tokenizer_new(const char *str, size_t str_size, enum whitespace_mode mode)
{
	Tokenizer *tok = calloc(1, sizeof(Tokenizer));
	tok->s_size = str_size;
	tok->s = str;
	tok->mode = mode;
	return tok;
}

void
Tokenizer_delete(Tokenizer *tok)
{
	free(tok->m_token);
	free(tok);
}

#if LUAXML_DEBUG
void
Tokenizer_print(Tokenizer *tok)
{
	printf("  @%u %s\n",
	       tok->i,
	       !tok->m_token ? "(null)"
	                     : (tok->m_token[0] == ESC)
	                           ? "(esc)"
	                           : (tok->m_token[0] == OPN)
	                                 ? "(open)"
	                                 : (tok->m_token[0] == CLS) ? "(close)"
	                                                            : tok->m_token);
	fflush(stdout);
}
#else
#define Tokenizer_print(tok) /* ignore */
#endif

static const char *
Tokenizer_set(Tokenizer *tok, const char *s, size_t size)
{
	if (!size || !s)
		return NULL;
	free(tok->m_token);
	tok->m_token = malloc(size + 1);
	strncpy(tok->m_token, s, size);
	tok->m_token[size] = 0;
	tok->m_token_size = tok->m_token_capacity = size;
	Tokenizer_print(tok);
	return tok->m_token;
}

static void
Tokenizer_append(Tokenizer *tok, char ch)
{
	if (tok->m_token_size + 1 >= tok->m_token_capacity) {
		tok->m_token_capacity =
		    tok->m_token_capacity ? tok->m_token_capacity * 2 : 16;
		tok->m_token = realloc(tok->m_token, tok->m_token_capacity);
	}
	tok->m_token[tok->m_token_size] = ch;
	tok->m_token[++tok->m_token_size] = 0;
}

const char *
Tokenizer_next(Tokenizer *tok)
{
	// NUL-terminated strings for the special tokens
	static const char ESC_str[] = {ESC, 0};
	static const char OPEN_str[] = {OPN, 0};
	static const char CLOSE_str[] = {CLS, 0};

	if (tok->m_token) {
		free(tok->m_token);
		tok->m_token = NULL;
		tok->m_token_size = tok->m_token_capacity = 0;
	}

	char quotMode = 0;
	int tokenComplete = 0;
	while (tok->m_next_size || (tok->i < tok->s_size)) {
		tok->cdata = 0;

		if (tok->m_next_size) {
			Tokenizer_set(tok, tok->m_next, tok->m_next_size);
			tok->m_next = NULL;
			tok->m_next_size = 0;
			return tok->m_token;
		}

		switch (tok->s[tok->i]) {
		case '"':
		case '\'':
			if (tok->tagMode) {
				// toggle quotation mode
				if (!quotMode)
					quotMode = tok->s[tok->i];
				else if (quotMode == tok->s[tok->i])
					quotMode = 0;
			}
			Tokenizer_append(tok, tok->s[tok->i]);
			break;

		case '<':
			if (!quotMode && (tok->i + 4 < tok->s_size)
			    && (strncmp(tok->s + tok->i, "<!--", 4) == 0))
				tok->i = find(tok->s, "-->", tok->i + 4) + 2; // strip comments
			else if (!quotMode && (tok->i + 9 < tok->s_size)
			         && (strncmp(tok->s + tok->i, "<![CDATA[", 9) == 0)) {
				if (tok->m_token_size > 0)
					// finish current token first, after that reparse CDATA
					tokenComplete = 1;
				else {
					// interpret CDATA
					size_t b = tok->i + 9;
					tok->i = find(tok->s, "]]>", b) + 3;
					size_t cdata_len = tok->i - b - 3;
					if (cdata_len > 0) {
						tok->cdata = 1; // mark as "raw" byte sequence
						return Tokenizer_set(tok, tok->s + b, cdata_len);
					}
				}
				--tok->i;
			} else if (!quotMode && (tok->i + 1 < tok->s_size)
			           && ((tok->s[tok->i + 1] == '?')
			               || (tok->s[tok->i + 1] == '!')))
				tok->i =
				    find(tok->s, ">", tok->i + 2); // strip meta information
			else if (!quotMode && !tok->tagMode) {
				if ((tok->i + 1 < tok->s_size) && (tok->s[tok->i + 1] == '/')) {
					// "</" sequence that starts a closing tag
					tok->m_next = ESC_str;
					tok->m_next_size = 1;
					tok->i = find(tok->s, ">", tok->i + 2);
				} else {
					// regular '<' opening a new tag
					tok->m_next = OPEN_str;
					tok->m_next_size = 1;
					tok->tagMode = 1;
				}
				tokenComplete = 1;
			} else
				Tokenizer_append(tok, tok->s[tok->i]);
			break;

		case '/':
			if (tok->tagMode && !quotMode) {
				tokenComplete = 1;
				if ((tok->i + 1 < tok->s_size) && (tok->s[tok->i + 1] == '>')) {
					// "/>" sequence = end of 'empty' tag
					tok->tagMode = 0;
					tok->m_next = ESC_str;
					tok->m_next_size = 1;
					++tok->i;
				} else
					Tokenizer_append(tok, tok->s[tok->i]);
			} else
				Tokenizer_append(tok, tok->s[tok->i]);
			break;

		case '>':
			if (!quotMode && tok->tagMode) {
				// this '>' closes the current tag
				tok->tagMode = 0;
				tokenComplete = 1;
				tok->m_next = CLOSE_str;
				tok->m_next_size = 1;
			} else
				Tokenizer_append(tok, tok->s[tok->i]);
			break;

		case ' ':
		case '\r':
		case '\n':
		case '\t':
			if (tok->tagMode && !quotMode) {
				// within a tag, any unquoted whitespace ends the current token
				// (= attribute)
				if (tok->m_token_size)
					tokenComplete = 1;
			} else if (tok->m_token_size || tok->mode != WHITESPACE_TRIM)
				Tokenizer_append(tok, tok->s[tok->i]);
			break;

		default:
			Tokenizer_append(tok, tok->s[tok->i]);
		}
		++tok->i;
		if (tok->i >= tok->s_size || (tokenComplete && tok->m_token_size)) {
			tokenComplete = 0;
			if (tok->mode == WHITESPACE_TRIM) // trim whitespace
				while (tok->m_token_size
				       && isspace(tok->m_token[tok->m_token_size - 1]))
					tok->m_token[--tok->m_token_size] = 0;
			if (tok->m_token_size)
				break;
		}
	}
	Tokenizer_print(tok);
	return tok->m_token;
}

//--- local variables ----------------------------------------------

// 'private' table mapping between special chars and their XML substitutions
static int sv_code_ref; // (will receive a LUA reference)

//--- public methods -----------------------------------------------

/** sets or returns tag of a LuaXML object.
This method is just "syntactic sugar" (using a typical Lua term) that allows
the writing of clearer code. LuaXML stores the tag value of an XML statement
at table index 0, hence it can be simply accessed or altered by `var[0]`.
However, writing `var:tag()` for access or `var:tag("newTag")` for altering
may be more self explanatory (and future-proof in case LuaXML's tag handling
should ever change).

@function tag
@param var  the variable whose tag should be accessed, a LuaXML object
@tparam ?string tag  the new tag to be set
@return  If you have passed a new tag, the function will return `var` (with
its tag changed); otherwise the result will be the current tag of `var`
(normally a string).
*/
int
Xml_tag(lua_State *L)
{
	// the function will only operate on tables
	if
		lua_istable(L, 1)
		{
			lua_settop(L, 2);
			push_TAG_key(L); // place tag key on top of stack (#3)
			if (lua_type(L, 2) == LUA_TSTRING) {
				lua_pushvalue(L, 2); // duplicate the value
				lua_rawset(L, 1);
				// we return the (modified) table
				lua_settop(L, 1);
				return 1;
			} else {
				// "tag" is empty or wrong type, retrieve the current tag
				lua_rawget(L, 1);
				return 1;
			}
		}
	return 0;
}

/** creates a LuaXML "object", and optionally sets its tag.
The function either sets the metatable of an existing Lua table, or creates a
new (empty) "object". If you pass an optional` tag` string, it will be assigned
to the result.

(It's also possible to call this as `new(tag)`, which creates a new XML object
with the given tag and is equivalent to `new({}, tag)`.)

Note that it's not mandatory to use this function in order to treat a Lua table
as LuaXML object. Setting the metatable just allows the usage of a more
object-oriented syntax (e.g. `xmlvar:str()` instead of `xml.str(xmlvar)`).
XML objects created by `load` or `eval` automatically offer the
object-oriented syntax.

@function new
@param arg (optional)  _(1)_ a table to be converted to a LuaXML object,
or _(2)_ the tag of the new LuaXML object
@tparam ?string tag  a tag value that will be assigned to the object
@return  LuaXML object, either newly created or the conversion of `arg`;
optionally tagged as requested
*/
int
Xml_new(lua_State *L)
{
	if (!lua_istable(L, 1)) {
		// create a new table and move it to the bottom of the stack (#1),
		// possibly shifting other elements "one up"
		lua_newtable(L);
		lua_insert(L, 1);
	}
	// element at #1 now is a table, convert to "object"
	make_xml_object(L, 1);

	if (lua_type(L, 2) == LUA_TSTRING) {
		lua_pushcfunction(L, Xml_tag);
		lua_pushvalue(L, 1); // duplicate the object table
		lua_pushvalue(L, 2); // duplicate the tag (string)
		lua_call(L, 2, 0);   // call the "tag" function, discarding any result
	}
	lua_settop(L, 1);
	return 1;
}

/** appends a new subordinate LuaXML object to an existing one.
optionally sets tag

@function append
@param var  the parent LuaXML object
@tparam ?string tag  the tag of the appended LuaXML object
@return  appended LuaXML object, or `nil` in case of errors
*/
int
Xml_append(lua_State *L)
{
	if (lua_type(L, 1) == LUA_TTABLE) {
		lua_settop(L, 2);
		lua_pushcfunction(L, Xml_new);
		lua_insert(L, 2);
		lua_call(L, 1, 1);                       // new(tag)
		lua_pushvalue(L, -1);                    // duplicate result
		lua_rawseti(L, 1, lua_rawlen(L, 1) + 1); // append to parent (elements)
		return 1;
	}
	return 0;
}

// Push XML-encoded string for the Lua value at given index.
// Will automatically use a tostring() conversion first, if necessary.
static void
Xml_pushEncode(lua_State *L, int index)
{
	if (index < 0)
		index += lua_gettop(L) + 1; // relative to absolute index
	if (lua_type(L, index) == LUA_TSTRING)
		lua_pushvalue(L, index); // already a string, just duplicate it
	else {
		lua_getglobal(L, "tostring");
		lua_pushvalue(L, index); // duplicate value
		lua_call(L, 1, 1);       // tostring()
	}

	// always do "&amp;" first
	// (avoids later affecting other substitutions, which may contain '&')
	do_gsub(L, -1, "&", "&amp;");

	// encode other special entities
	lua_rawgeti(L, LUA_REGISTRYINDEX, sv_code_ref);
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		// Lua stack has string to work on (-4), substitution table (-3),
		// table key (-2 = special char) and value (-1 = replacement)
		// (We want to replace the original char with the XML encoding.)
		do_gsub(L, -4, lua_tostring(L, -2), lua_tostring(L, -1));
		lua_pop(L, 1); // pop value, leaving key for the next iteration
	}
	lua_pop(L, 1); // pop substitution table to realign the stack

	// transfer string one character at a time, encoding any chars with MSB set
	char buf[8];
	const unsigned char *s = (unsigned char *)lua_tostring(L, -1);
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	while (*s) {
		if (*s < 128)
			luaL_addchar(&b, *s); // copy character literally
		else {
			int len = snprintf(buf, sizeof(buf), "&#%d;", *s); // encode char
			luaL_addlstring(&b, buf, len);
		}
		s++;
	}
	luaL_pushresult(&b);
	lua_replace(L, -2); // (leaving the result on the stack)
}

/*
// Push a string, then do XML conversion on it - result remains on top of stack.
static void Xml_pushEncodeStr(lua_State *L, const char *s, int size) {
    if (size == 0) {
        lua_pushliteral(L, "");
        return;
    }
    if (size < 0) size = strlen(s);
    lua_pushlstring(L, s, size);
    Xml_pushEncode(L, -1);
    lua_replace(L, -2);
}
*/

// Push Lua representation of the given string, while decoding any special XML
// encodings
static void
Xml_pushDecode(lua_State *L, const char *s, int size)
{
	if (size == 0) {
		lua_pushliteral(L, "");
		return;
	}
	if (size < 0)
		size = strlen(s);

	// try a gsub() substition of decimal and hexadecimal character encodings
	lua_pushlstring(L, s, size); // initial string
	lua_pushliteral(L, "gsub");
	lua_gettable(L, -2); // using string as object, retrieve the "gsub" function
	lua_insert(L, -2);   // swap with function, making string the arg #1
	lua_pushliteral(L, "&#(x?%x+);"); // pattern for XML encodings (arg #2)
	lua_pushcfunction(L, XMLencoding_replacement); // replacement func (arg #3)
	lua_call(L, 3, 1); // three parameters, one result (the substituted string)

	lua_rawgeti(L, LUA_REGISTRYINDEX, sv_code_ref);
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		// Lua stack has string to work on (-4), substitution table (-3),
		// table key (-2 = special char) and value (-1 = replacement)
		// (We want to replace the XML encoding with the original char.)
		do_gsub(L, -4, lua_tostring(L, -1), lua_tostring(L, -2));
		lua_pop(L, 1); // pop value, leaving key for the next iteration
	}
	lua_pop(L, 1); // pop substitution table, leaving result string on stack
	do_gsub(L, -1, "&amp;", "&"); // this should always be done last
}

/** parses an XML string into a Lua table.
The table will contain a representation of the XML tag, attributes (and their
values), and element content / subelements (either as strings or nested LuaXML
"objects").

Note: Parsing "wide" strings or Unicode (UCS-2, UCS-4, UTF-16) currently is
__not__ supported. If needed, convert such `xml` data to UTF-8 before passing it
to `eval()`. UTF-8 should be safe to use, and this function will also recognize
and ignore a UTF-8 BOM (byte order mark) at the start of `xml`.

@function eval

@tparam string|userdata xml
the XML to be converted. When passing a userdata type `xml` value, it must
point to a C-style (NUL-terminated) string.

@tparam ?number mode
whitespace handling mode, one of the `WS_*` constants - see [Fields](#Fields).
defaults to `WS_TRIM` (compatible to previous LuaXML versions)

@return  a LuaXML object containing the XML data, or `nil` in case of errors
*/
int
Xml_eval(lua_State *L)
{
	enum whitespace_mode mode = luaL_optint(L, 2, WHITESPACE_TRIM);
	const char *str;
	size_t str_size;
	if (lua_isuserdata(L, 1)) {
		str = lua_touserdata(L, 1);
		str_size = strlen(str);
	} else
		str = luaL_checklstring(L, 1, &str_size);

	if (str_size >= 3 && strncmp(str, "\xEF\xBB\xBF", 3) == 0) {
		// ignore / skip over UTF-8 BOM (byte order mark)
		str += 3;
		str_size -= 3;
	}

	Tokenizer *tok = Tokenizer_new(str, str_size, mode);
	lua_settop(L, 1);
	const char *token;
	int firstStatement = 1;
	while ((token = Tokenizer_next(tok)))
		if (*token == OPN) { // new tag found
			if (lua_gettop(L) > 1) {
				lua_newtable(L);
				lua_pushvalue(L,
				              -1); // duplicate table (keep one copy on stack)
				lua_rawseti(L,
				            -3,
				            lua_rawlen(L, -3) + 1); // set parent subelement
			} else {
				if (firstStatement) {
					lua_newtable(L);
					firstStatement = 0;
				} else
					return 0;
			}
			make_xml_object(L, -1); // assign metatable

			// parse tag and content:
			push_TAG_key(L); // place tag key on top of stack
			lua_pushstring(L, Tokenizer_next(tok));
			lua_rawset(L, -3);

			while ((token = Tokenizer_next(tok)) && (*token != CLS)
			       && (*token != ESC)) {
				// parse tag header
				size_t sepPos = find(token, "=", 0);
				if (token[sepPos]) { // regular attribute (key="value")
					const char *aVal = token + sepPos + 2;
					lua_pushlstring(L, token, sepPos);
					Xml_pushDecode(L, aVal, strlen(aVal) - 1);
					lua_rawset(L, -3);
				}
			}
			if (!token || (*token == ESC)) {
				// this tag has no content, only attributes
				if (lua_gettop(L) > 2)
					lua_pop(L, 1);
				else
					break;
			}
		} else if (*token == ESC) { // previous tag is over
			if (lua_gettop(L) > 2)
				lua_pop(L, 1); // pop current table
			else
				break;
		} else { // read elements
			if (lua_gettop(L) > 1) {
				// when normalizing, we ignore tokens considered "lead-in" type
				if (mode != WHITESPACE_NORMALIZE || !is_lead_token(token)) {
					if (tok->cdata) // "raw" mode, don't change token string!
						lua_pushstring(L, token);
					else
						Xml_pushDecode(L, token, -1);
					lua_rawseti(L, -2, lua_rawlen(L, -2) + 1);
				}
			} else // element stack is empty, i.e. we encountered a token
			       // *before* any tag
			    if (!is_whitespace(token))
				luaL_error(L,
				           "Malformed XML: non-empty string '%s' before any "
				           "tag (parser pos %d)",
				           token,
				           (int)tok->i);
		}
	Tokenizer_delete(tok);
	return lua_gettop(L) - 1;
}

/** loads XML data from a file and returns it as table.
Basically, this is just calling `eval` on the given file's content.

@function load
@tparam string filename  the name and path of the file to be loaded
@tparam ?number mode  whitespace handling mode, defaults to `WS_TRIM`
@return  a Lua table representing the XML data, or `nil` in case of errors
*/
int
Xml_load(lua_State *L)
{
	const char *filename = luaL_checkstring(L, 1);
	FILE *file = fopen(filename, "r");
	if (!file)
		return luaL_error(L,
		                  "LuaXML ERROR: \"%s\" file error or file not found!",
		                  filename);

	fseek(file, 0, SEEK_END);
	size_t sz = ftell(file);
	rewind(file);
	char *buffer = malloc(sz + 1);
	sz = fread(buffer, 1, sz, file);
	fclose(file);
	buffer[sz] = 0;
	lua_pushlightuserdata(L, buffer);
	lua_replace(L, 1);
	int result = Xml_eval(L);
	free(buffer);
	return result;
};

/** registers a custom code for the conversion between non-standard characters
and XML character entities.

By default, only the most basic entities are known to LuaXML:
    " < > '
On top (and independent) of that, the **ampersand** sign always gets encoded /
decoded separately: `&amp;` &harr; `&amp;amp;`. Character codes above 127 are
directly converted to an appropriate XML encoding, representing the character
number (e.g. `&amp;#160;`). If other special encodings are needed, they can be
registered using this function.

Note: LuaXML now manages these encodings in a (private) standard Lua table.
This allows you to replace entries by calling `registerCode()` again, using the
same `decoded` and a different `encoded`. Encodings may even be removed later,
by explictly registering a `nil` value: `registerCode(decoded, nil)`.

@function registerCode
@tparam string decoded  the character (sequence) to be used within Lua
@tparam string encoded  the character entity to be used in XML
@see encode, decode
*/
int
Xml_registerCode(lua_State *L)
{
	// We require the "decoded" string, but allow `nil` as argument #2.
	// That way, users may remove entries from the table again.
	luaL_checkstring(L, 1);
	if (!lua_isnoneornil(L, 2))
		luaL_checkstring(L, 2);

	lua_settop(L, 2);
	lua_rawgeti(L, LUA_REGISTRYINDEX, sv_code_ref); // get translation table
	lua_insert(L, 1);
	lua_rawset(L, 1); // assign key-value pair (k "decoded" -> v "encoded")
	return 0;
}

/** converts a string to XML encoding.
This function transforms` str` by replacing any special characters with
suitable XML encodings.

@usage
print(xml.encode("<->")) -- "&lt;-&gt;"

@function encode
@tparam string str  string to be transformed
@treturn string  the XML-encoded string
@see decode, registerCode
*/
int
Xml_encode(lua_State *L)
{
	luaL_checkstring(L, 1); // make sure arg #1 is a string
	Xml_pushEncode(L, 1);   // and convert it
	return 1;
}

/** converts a string from XML encoding.
This function transforms` str` by replacing any special XML encodings with
their "plain text" counterparts.

@usage
print((xml.decode("&lt;-&gt;")) -- "<->"

@function decode
@tparam string str  string to be transformed
@treturn string  the decoded string
@see encode, registerCode
*/
int
Xml_decode(lua_State *L)
{
	size_t size;
	luaL_checklstring(L, 1, &size);              // make sure arg #1 is a string
	Xml_pushDecode(L, lua_tostring(L, 1), size); // and convert it
	return 1;
}

/** converts any Lua value to an XML string.
@function str

@param value
the value to be converted, normally a table (LuaXML object). However this
function will 'encapsulate' other Lua values (of arbitrary type) in a way that
should make them valid XML.
<br>Note: Passing no `value` will cause the function to return `nil`.

@tparam ?number indent
indentation level for 'pretty' output. Mainly for internal use, defaults to 0.

@tparam ?string tag
the tag to be used in case `value` doesn't already have an 'implicit' tag.
Mainly for internal use.

@treturn string
an XML string, or `nil` in case of errors.
*/
int
Xml_str(lua_State *L)
{
	// Note:
	// Be very careful about mixing Lua stack usage and buffer access here.
	// The stack *must* be (re)balanced before accessing "b", i.e. any output
	// should only occur at the same Lua stack level as the previous one!
	luaL_Buffer b;

	lua_settop(L, 3);
	int type = lua_type(L, 1); // type of "value"
	if (type == LUA_TNIL)
		return 0;

	if (type == LUA_TTABLE) {
		push_TAG_key(L);
		lua_rawget(L, 1); // retrieve tag entry from the table (may be `nil`)

		// order of precedence: value[0], explicit tag string, Lua type name
		const char *tag = lua_tostring(L, -1);
		if (!tag)
			tag = lua_tostring(L, 3);
		if (!tag)
			tag = lua_typename(L, type);

		// Four elements already on stack: value, indent, tag, value[0]
		// Use a string (#5) to manage (concatenate) simple attributes
		lua_pushliteral(L, "");
		// And a table (#6) to take care of (collect) 'extended' attributes
		lua_newtable(L);
		size_t table_attr = 0;

		luaL_buffinit(L, &b);
		push_indentStr(L, lua_tointeger(L, 2));
		luaL_addvalue(&b);
		luaL_addchar(&b, '<');
		luaL_addstring(&b, tag);

		// Iterate over string keys (= attributes)
		lua_pushnil(L);
		while (lua_next(L, 1)) {
			// (k, v) pair on the stack
			if (lua_type(L, -2) == LUA_TSTRING) {
				// (the "_M" test here is to avoid recursion on module tables)
				if (lua_istable(L, -1) && strcmp(lua_tostring(L, -2), "_M")) {
					lua_pushcfunction(L, Xml_str);
					lua_pushvalue(L, -2); // duplicate "v"
					lua_pushinteger(L, lua_tointeger(L, 2) + 1); // indent + 1
					lua_pushvalue(L, -4); // duplicate "k"
					lua_call(L, 3, 1);    // xml.str(v, indent + 1, k)
					lua_rawseti(L, 6, ++table_attr); // append string to table
				} else {
					Xml_pushEncode(L, -1); // encode(tostring(v))
					lua_pushfstring(L,
					                "%s %s=\"%s\"",
					                lua_tostring(L, 5),
					                lua_tostring(L, -3),
					                lua_tostring(L, -1));
					lua_replace(L, 5); // new attribute string
					lua_pop(L, 1);     // realign stack
				}
			}
			lua_pop(L, 1); // pop <v>alue, leaving <k>ey for next iteration
		}
		// append "simple" attribute string to the output
		if (lua_rawlen(L, 5) > 0)
			luaL_addstring(&b, lua_tostring(L, 5));

		size_t count = lua_rawlen(L, 1); // number of "array" (sub)elements
		if (count == 0 && table_attr == 0) {
			// no sub-elements and no extended attr -> close tag and we're done
			luaL_addlstring(&b, " />\n", 4);
			luaL_pushresult(&b);
			return 1;
		}
		luaL_addchar(&b, '>'); // close opening tag
		if (count == 1 && table_attr == 0) {
			// single subelement, no extended attributes
			lua_rawgeti(L, 1, 1); // value[1]
			if (!lua_istable(L, -1)) {
				// output as single string, then close tag
				Xml_pushEncode(L, -1); // encode(tostring(value[1]))
				lua_replace(L, -2);
				luaL_addvalue(&b); // add and pop
				luaL_addlstring(&b, "</", 2);
				luaL_addstring(&b, tag);
				luaL_addlstring(&b, ">\n", 2);
				luaL_pushresult(&b);
				return 1;
			}
			lua_pop(L, 1); // discard (table) value, to realign stack
		}
		luaL_addchar(&b, '\n');

		// Loop over all the sub-elements, placing each on a separate line
		size_t k;
		for (k = 1; k <= count; k++) {
#if LUA_VERSION_NUM < 503
			lua_rawgeti(L, 1, k);
			type = lua_type(L, -1);
#else
			type = lua_rawgeti(L, 1, k); // (Lua 5.3 returns type directly)
#endif
			if (type == LUA_TSTRING) {
				push_indentStr(L, lua_tointeger(L, 2) + 1); // indentation
				Xml_pushEncode(L, -2);
				lua_remove(L, -3);
				lua_pushliteral(L, "\n");
				lua_concat(L, 3);
			} else {
				lua_pushcfunction(L, Xml_str);
				lua_insert(L, -2); // place function before value
				lua_pushinteger(L, lua_tointeger(L, 2) + 1); // indent + 1
				lua_call(L, 2, 1); // xml.str(v, indent + 1)
			}
			luaL_addvalue(&b); // add (string) to output, pop from stack
		}

		// Finally we'll take care of the "extended" (table-type) attributes.
		// The output is appended after the regular sub-elements, in order
		// not to affect their numbering.
		// Just process the corresponding table, concatenating all entries:
		for (k = 1; k <= table_attr; k++) {
			lua_rawgeti(L, 6, k);
			luaL_addvalue(&b);
		}

		// closing tag
		push_indentStr(L, lua_tointeger(L, 2));
		luaL_addvalue(&b);
		luaL_addlstring(&b, "</", 2);
		luaL_addstring(&b, tag);
		luaL_addlstring(&b, ">\n", 2);

		luaL_pushresult(&b);
		return 1;
	}

	// Getting here means a "flat" Lua value, format to XML as a single string
	const char *tag = lua_tostring(L, 3);
	if (!tag)
		tag = lua_typename(L, type); // use either tag or the type name
	luaL_buffinit(L, &b);
	push_indentStr(L, lua_tointeger(L, 2));
	luaL_addvalue(&b);
	luaL_addchar(&b, '<');
	luaL_addstring(&b, tag);
	luaL_addchar(&b, '>');

	Xml_pushEncode(L, 1); // encode(tostring(value))
	luaL_addvalue(&b);

	luaL_addlstring(&b, "</", 2);
	luaL_addstring(&b, tag);
	luaL_addlstring(&b, ">\n", 2);
	luaL_pushresult(&b);
	return 1;
}

/** match XML entity against given (optional) criteria.
Passing `nil` for one of the` tag`, `key`, or `value` parameters means "don't
care" (i.e. match anything for that particular aspect). So for example
    var:match(nil, "text", nil)
    -- or shorter, but identical:  var:match(nil, "text")
will look for an XML attribute (name) "text" to be present in `var`, but won't
consider its value or the tag of `var`.

Note: If you want to test for a specific attribute `value`, so also have to
supply a `key` - otherwise `value` will be ignored.

@usage
-- each of these will either return `x`, or `nil` in case of no match

x:match("foo") -- test for x:tag() == "foo"
x:match(nil, "bar") -- test if x has a "bar" attribute
x:match(nil, "foo", "bar") -- test if x has a "foo" attribute that equals "bar"
x:match("foobar", "foo", "bar") -- test for "foobar" tag, and attr "foo" ==
"bar"

@function match

@param var
the variable to test, normally a Lua table or LuaXML object. (If `var` is not
a table type, the test always fails.)

@tparam ?string tag
If set, has to match the XML `tag` (i.e. must be equal to the `tag(var, nil)`
result)

@tparam ?string key
If set, a corresponding **attribute key** needs to be present (exact name
match).

@param value (optional)
arbitrary Lua value. If set, the **attribute value** for `key` has to match it.

@return
either `nil` for no match; or the `var` argument properly converted to a
LuaXML object, equivalent to `xml.new(var)`.

This allows you to either make direct use of the matched LuaXML object, or to
use the return value in a boolean test (`if xml.match(...)`), which is a common
Lua idiom.
*/
int
Xml_match(lua_State *L)
{
	if (lua_type(L, 1) == LUA_TTABLE) {
		if (!lua_isnoneornil(L, 2)) {
			push_TAG_key(L);
			lua_rawget(L, 1); // get the tag value from var
			if (!lua_equal(L, -1, 2))
				return 0;  // tag mismatch, return `nil`
			lua_pop(L, 1); // realign stack
		}
		if (lua_type(L, 3) == LUA_TSTRING) {
			lua_pushvalue(L, 3); // duplicate attribute key
			lua_rawget(L, 1);    // try to get value from var
			if (lua_isnil(L, -1))
				return 0; // no such attribute
			if (!lua_isnoneornil(L, 4)) {
				if (!lua_equal(L, -1, 4))
					return 0; // attribute value mismatch
			}
		}
		lua_settop(L, 1);
		make_xml_object(L, 1);
		return 1;
	}
	return 0;
}

/** iterates a LuaXML object,
invoking a callback function for all matching (sub)elements.

The iteration starts with the variable `var` itself (= default depth 0).
A callback function `cb` gets invoked for each `match`, depending on the
specified criteria. If the `r` flag is set, the process will
repeat **recursively** for the subelements of `var` (at depth + 1). You can
limit the scope by setting a maximum depth, or have the callback function
explicitly request to stop the iteration (by returning `false`).

@function iterate

@param var  the table (LuaXML object) to iterate

@tparam function cb
callback function. `callback(var, depth)` will be called for each matching
element.<br>
The function may return `false` to request a stop; if its result is
any other value (including `nil`), the iteration will continue.

@tparam ?string tag  XML tag to be matched
@tparam ?string key  attribute key to be matched
@param value  (optional) attribute value to be matched

@tparam ?boolean r
recursive operation. If `true`, also iterate over the subelements of `var`

@tparam ?number max  maximum depth allowed
@tparam ?number d  initial depth value, defaults to 0

@return
The function returns two values: a counter representing the number of elements
that were successfully matched (and processed), and a boolean completion flag.
The latter is `true` for an exhaustive iteration, and `false` if was stopped
from the callback.

@see match
*/
int
Xml_iterate(lua_State *L)
{
	lua_settop(L, 8);
	luaL_checktype(L, 2, LUA_TFUNCTION);  // callback must be a function
	int maxdepth = luaL_optint(L, 7, -1); // default (< 0) indicates "no limit"
	int depth = lua_tointeger(L, 8);
	int count = 0;
	bool cont = true;
	// examine "var" element first
	lua_pushcfunction(L, Xml_match);
	lua_pushvalue(L, 1); // var
	lua_pushvalue(L, 3); // tag
	lua_pushvalue(L, 4); // key
	lua_pushvalue(L, 5); // value
	lua_call(L, 4, 1);
	if (!lua_isnil(L, -1)) { // "var" matches, invoke callback
		count = 1;
		lua_pushvalue(L, 2); // duplicate function
		lua_insert(L, -2);
		lua_pushinteger(L, depth);
		lua_call(L, 2, 1);
		lua_pushboolean(L, false);
		cont = !lua_equal(L, -1, -2);
		lua_pop(L, 2);
	} else
		lua_pop(L, 1);
	if (cont && lua_toboolean(L, 6) && lua_type(L, 1) == LUA_TTABLE) {
		// process "children" / sub-elements recursively
		depth += 1;
		if (maxdepth < 0 || depth <= maxdepth) {
			int k = 0;
			while (true) {
				lua_pushcfunction(L, Xml_iterate);
				lua_rawgeti(L, 1, ++k);
				if (lua_isnil(L, -1))
					break; // no element var[k], exit loop
				lua_pushvalue(L, 2);
				lua_pushvalue(L, 3);
				lua_pushvalue(L, 4);
				lua_pushvalue(L, 5);
				lua_pushboolean(L, true);
				lua_pushvalue(L, 7);
				lua_pushinteger(L, depth);
				lua_call(L, 8, 2); // done, continue = iterate(var[k], ...)
				count += lua_tointeger(L, -2);
				if (!lua_toboolean(L, -1)) {
					lua_pushinteger(L, count);
					lua_pushboolean(L, false);
					return 2;
				}
				lua_pop(L, 2);
			}
		}
	}
	lua_pushinteger(L, count);
	lua_pushboolean(L, true);
	return 2;
}

/** recursively searches a Lua table for a subelement
matching the provided tag and attribute. See the description of `match` for
the logic involved with testing for` tag`, `key` and `value`.

@function find
@param var  the table to be searched in
@tparam ?string tag  the XML tag to be found
@tparam ?string key  the attribute key (= exact name) to be found
@param value (optional)  the attribute value to be found
@return  the first (sub-)table that satisfies the search condition,
or `nil` for no match
*/
int
Xml_find(lua_State *L)
{
	lua_settop(L, 4); // accept at most four parameters for this function

	lua_newtable(L); // upon a match, this table will receive our result as t[1]
	lua_insert(L, 1); // (move it before anything else)

	lua_pushcfunction(L, Xml_iterate);
	lua_insert(L, 2);    // iterate is now stack arg #2, `var` at #3
	lua_pushvalue(L, 1); // duplicate the table (for use as upvalue)
	lua_pushcclosure(L, find_on_match, 1); // create a C closure
	lua_insert(L, 4); // place the callback function (closure) at #4
	// (`tag`, `key` and `value` have moved to #5, #6 and #7 respectively)
	lua_pushboolean(L, true); // set "recursive" flag (#8)

	// iterate(var, find_on_match, tag, key, value, true), discarding results
	// (but if something matches, we expect that `find_on_match` sets t[1])
	lua_call(L, 6, 0);
	lua_rawgeti(L, 1, 1);
	return 1; // returns result[1], which may be `nil` (if no match)
}

#ifdef __cplusplus
extern "C" {
#endif
int _EXPORT
luaopen_LuaXML_lib(lua_State *L)
{
	static const struct luaL_Reg funcs[] = {{"append", Xml_append},
	                                        {"decode", Xml_decode},
	                                        {"encode", Xml_encode},
	                                        {"eval", Xml_eval},
	                                        {"find", Xml_find},
	                                        {"iterate", Xml_iterate},
	                                        {"load", Xml_load},
	                                        {"match", Xml_match},
	                                        {"new", Xml_new},
	                                        {"registerCode", Xml_registerCode},
	                                        {"str", Xml_str},
	                                        {"tag", Xml_tag},
	                                        {NULL, NULL}};
	luaL_newlib(L, funcs);

	// create a metatable for LuaXML "objects"
	luaL_newmetatable(L, LUAXML_META);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3); // duplicate the module table
	lua_rawset(L, -3);    // and set it as metaindex
	lua_pushliteral(L, "__tostring");
	lua_pushcfunction(L, Xml_str);
	lua_rawset(L, -3); // set metamethod
	lua_pop(L, 1);     // drop value (metatable)

	// expose API constants (via the module table)
	lua_pushinteger(L, WHITESPACE_TRIM);
	lua_setfield(L, -2, "WS_TRIM");
	lua_pushinteger(L, WHITESPACE_NORMALIZE);
	lua_setfield(L, -2, "WS_NORMALIZE");
	lua_pushinteger(L, WHITESPACE_PRESERVE);
	lua_setfield(L, -2, "WS_PRESERVE");

	// register default codes
	// Note: We'll always handle "&amp;" separately!
	lua_newtable(L);
	lua_pushliteral(L, "&lt;");
	lua_setfield(L, -2, "<");
	lua_pushliteral(L, "&gt;");
	lua_setfield(L, -2, ">");
	lua_pushliteral(L, "&quot;");
	lua_setfield(L, -2, "\"");
	lua_pushliteral(L, "&apos;");
	lua_setfield(L, -2, "'");
	sv_code_ref = luaL_ref(L, LUA_REGISTRYINDEX); // reference (and pop table)

	return 1; // return module (table)
}
#ifdef __cplusplus
} // extern "C"
#endif
