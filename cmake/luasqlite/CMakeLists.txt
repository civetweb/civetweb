cmake_minimum_required(VERSION 2.8.11)
project(luasqlite)
add_library(luasqlite STATIC lsqlite3.c)
install(
  TARGETS luasqlite
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT library)
