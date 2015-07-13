cmake_minimum_required(VERSION 2.8.11)
project(luafilesystem)
add_library(luafilesystem STATIC src/lfs.c)
target_include_directories(
  luafilesystem PUBLIC
  src)
install(
  TARGETS luafilesystem
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT library)
