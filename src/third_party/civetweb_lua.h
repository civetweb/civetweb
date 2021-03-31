/* Copyright (c) 2015-2021 the Civetweb developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* This header is intended to support Lua 5.1, Lua 5.2 and Lua 5.3 in the same
 * C source code.
 */

#ifndef CIVETWEB_LUA_H
#define CIVETWEB_LUA_H

#define LUA_LIB
#define LUA_COMPAT_LOG10
#define LUA_COMPAT_APIINTCASTS

#if defined(__cplusplus)
extern "C" {
#endif
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif

#ifndef LUA_VERSION_NUM
#error "Unknown Lua version"

#elif LUA_VERSION_NUM == 501
/* Lua 5.1 detected */
#define LUA_OK 0
#define LUA_ERRGCMM 999 /* not supported */
#define mg_lua_load(a, b, c, d, e) lua_load(a, b, c, d)
#define lua_rawlen lua_objlen
#define lua_newstate(a, b)                                                     \
	luaL_newstate() /* Must use luaL_newstate() for 64 bit target */
#define lua_pushinteger lua_pushnumber
#define luaL_newlib(L, t)                                                      \
	{                                                                          \
		luaL_Reg const *r = t;                                                 \
		while (r->name) {                                                      \
			lua_register(L, r->name, r->func);                                 \
			r++;                                                               \
		}                                                                      \
	}
#define luaL_setfuncs(L, r, u) lua_register(L, r->name, r->func)

#elif LUA_VERSION_NUM == 502
/* Lua 5.2 detected */
#define mg_lua_load lua_load

#elif LUA_VERSION_NUM == 503
/* Lua 5.3 detected */
#define mg_lua_load lua_load

#elif LUA_VERSION_NUM == 504
/* Lua 5.4 detected */
#define mg_lua_load lua_load

#else
#error "Lua version not supported (yet?)"

#endif

#if defined(LUA_VERSION_MAKEFILE)
#if LUA_VERSION_MAKEFILE != LUA_VERSION_NUM
#error                                                                         \
    "Mismatch between Lua version specified in Makefile and Lua version in lua.h"
#endif
#endif

#endif /* #ifndef CIVETWEB_LUA_H */
