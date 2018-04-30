/* Copyright (c) 2018 CivetWeb developers
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


/* Shared data for all Lua states */
static lua_State *L_shared;
static pthread_mutex_t lua_shared_lock;

void
lua_shared_init(void)
{
	L_shared = lua_newstate(lua_allocator, NULL);
	printf("Lsh: %p\n", L_shared);

	lua_newtable(L_shared);
	lua_setglobal(L_shared, "shared");

	pthread_mutex_init(&lua_shared_lock, &pthread_mutex_attr);
}


void
lua_shared_exit(void)
{
	printf("Lsh: %p\n", L_shared);

	lua_close(L_shared);
	L_shared = 0;

	pthread_mutex_destroy(&lua_shared_lock);
}


static int
lua_shared_index(struct lua_State *L)
{
	void *ud = lua_touserdata(L, 1);
	int key_type = lua_type(L, 2);
	int val_type;

	printf("lua_shared_index call (%p)\n", ud);

	if ((key_type != LUA_TNUMBER) && (key_type != LUA_TSTRING)
	    && (key_type != LUA_TBOOLEAN)) {
		return luaL_error(L, "shared index must be string, number or boolean");
	}

	pthread_mutex_lock(&lua_shared_lock);

	lua_getglobal(L_shared, "shared");

	if (key_type == LUA_TNUMBER) {
		double num = lua_tonumber(L, 2);
		printf("index: %G\n", num);
		lua_pushnumber(L_shared, num);
	} else if (key_type == LUA_TBOOLEAN) {
		int i = lua_toboolean(L, 2);
		printf("index: %s\n", i ? "true" : "false");
		lua_pushboolean(L_shared, i);
	} else {
		size_t len = 0;
		const char *str = lua_tolstring(L, 2, &len);
		printf("index: %s\n", str);
		lua_pushlstring(L_shared, str, len);
	}

	lua_rawget(L_shared, -2);

	val_type = lua_type(L_shared, -1);

	if (val_type == LUA_TNUMBER) {
		double num = lua_tonumber(L_shared, -1);
		printf("value: %G\n", num);
		lua_pushnumber(L, num);
	} else if (val_type == LUA_TBOOLEAN) {
		int i = lua_toboolean(L_shared, -1);
		printf("value: %s\n", i ? "true" : "false");
		lua_pushboolean(L, i);
	} else if (val_type == LUA_TNIL) {
		lua_pushnil(L);
	} else {
		size_t len = 0;
		const char *str = lua_tolstring(L_shared, -1, &len);
		printf("value: %s\n", str);
		lua_pushlstring(L, str, len);
	}

	lua_pop(L_shared, 2);

	pthread_mutex_unlock(&lua_shared_lock);

	return 1;
}


static int
lua_shared_newindex(struct lua_State *L)
{
	void *ud = lua_touserdata(L, 1);
	int key_type = lua_type(L, 2);
	int val_type = lua_type(L, 3);

	printf("lua_shared_newindex call (%p)\n", ud);

	if ((key_type != LUA_TNUMBER) && (key_type != LUA_TSTRING)
	    && (key_type != LUA_TBOOLEAN)) {
		return luaL_error(L, "shared index must be string, number or boolean");
	}
	if ((val_type != LUA_TNUMBER) && (val_type != LUA_TSTRING)
	    && (val_type != LUA_TBOOLEAN)) {
		return luaL_error(L, "shared value must be string, number or boolean");
	}

	pthread_mutex_lock(&lua_shared_lock);

	lua_getglobal(L_shared, "shared");

	if (key_type == LUA_TNUMBER) {
		double num = lua_tonumber(L, 2);
		printf("index: %G\n", num);
		lua_pushnumber(L_shared, num);
	} else if (key_type == LUA_TBOOLEAN) {
		int i = lua_toboolean(L, 2);
		printf("index: %s\n", i ? "true" : "false");
		lua_pushboolean(L_shared, i);
	} else {
		size_t len = 0;
		const char *str = lua_tolstring(L, 2, &len);
		printf("index: %s\n", str);
		lua_pushlstring(L_shared, str, len);
	}

	if (val_type == LUA_TNUMBER) {
		double num = lua_tonumber(L, 3);
		printf("index: %G\n", num);
		lua_pushnumber(L_shared, num);
	} else if (val_type == LUA_TBOOLEAN) {
		int i = lua_toboolean(L, 3);
		printf("index: %s\n", i ? "true" : "false");
		lua_pushboolean(L_shared, i);
	} else {
		size_t len = 0;
		const char *str = lua_tolstring(L, 3, &len);
		printf("index: %s\n", str);
		lua_pushlstring(L_shared, str, len);
	}

	lua_rawset(L_shared, -3);
	lua_pop(L_shared, 1);

	pthread_mutex_unlock(&lua_shared_lock);

	return 0;
}


void
lua_shared_register(struct lua_State *L)
{
	lua_newuserdata(L, 0);
	lua_newtable(L);

	lua_pushliteral(L, "__index");
	lua_pushcclosure(L, lua_shared_index, 0);
	lua_rawset(L, -3);

	lua_pushliteral(L, "__newindex");
	lua_pushcclosure(L, lua_shared_newindex, 0);
	lua_rawset(L, -3);

	lua_setmetatable(L, -2);
	lua_setglobal(L, "shared");
}
