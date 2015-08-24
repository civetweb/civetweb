/* "civetweb_private_lua.h" */
/* Project internal header to allow main.c to call a non-public function in
 * mod_lua.inl */

#ifndef CIVETWEB_PRIVATE_LUA_H
#define CIVETWEB_PRIVATE_LUA_H

void civetweb_open_lua_libs(lua_State *L);

#endif
