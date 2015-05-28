cmake_minimum_required(VERSION 2.8.11)
project(sqlite)
add_library(sqlite STATIC sqlite3.c)
target_include_directories(
  sqlite PUBLIC
  .)
install(
  TARGETS sqlite
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT library)
