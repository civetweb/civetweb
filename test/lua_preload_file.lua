--[[
Load this test file by adding  
  lua_preload_file ./lua_preload_file.lua
to the civetweb.conf file
]]

mg.preload = "lua_preload_file successfully loaded"
