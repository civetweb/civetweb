cmake_minimum_required(VERSION 2.8.11)
project(luaxml)
add_library(luaxml STATIC LuaXML_lib.c)
install(
  TARGETS luaxml
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT library)
