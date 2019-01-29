// Registers the entire luasocket library

#include <string.h>
extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include "luasocket.h"
	#include "mime.h"
}





// The Lua sources stored in separate cpp files:
extern "C" extern const char LuaSrc_ftp[];
extern "C" extern const char LuaSrc_headers[];
extern "C" extern const char LuaSrc_http[];
extern "C" extern const char LuaSrc_ltn12[];
extern "C" extern const char LuaSrc_mime[];
extern "C" extern const char LuaSrc_smtp[];
extern "C" extern const char LuaSrc_socket[];
extern "C" extern const char LuaSrc_tp[];
extern "C" extern const char LuaSrc_url[];





void requirestring(lua_State * L, const char * aModuleName, const char * aCode)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
	lua_getfield(L, -1, aModuleName);  /* LOADED[aModuleName] */
	if (!lua_toboolean(L, -1))  /* package not already loaded? */
	{
		lua_pop(L, 1);  /* remove field */
		luaL_loadbuffer(L, aCode, strlen(aCode), aModuleName);
		lua_call(L, 0, 1);
		lua_pushvalue(L, -1);  /* make copy of module (call result) */
		lua_setfield(L, -3, aModuleName);  /* LOADED[aModuleName] = module */
	}
	lua_remove(L, -2);  /* remove LOADED table */
}





extern "C" void luaopen_luasocket(lua_State * L)
{
	luaL_requiref(L, "socket.core", luaopen_socket_core, false);
	luaL_requiref(L, "mime.core",   luaopen_mime_core, false);
	requirestring(L, "socket", LuaSrc_socket);
	requirestring(L, "ltn12",  LuaSrc_ltn12);
	requirestring(L, "mime",   LuaSrc_mime);
	requirestring(L, "socket.url",     LuaSrc_url);
	requirestring(L, "socket.headers", LuaSrc_headers);
	requirestring(L, "socket.http",    LuaSrc_http);
	requirestring(L, "socket.tp",      LuaSrc_tp);
	requirestring(L, "socket.ftp",     LuaSrc_ftp);
	requirestring(L, "socket.smtp",    LuaSrc_smtp);
}
