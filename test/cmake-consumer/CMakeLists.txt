cmake_minimum_required(VERSION 3.10)
project(civetweb_consumer LANGUAGES C CXX)

find_package(civetweb CONFIG REQUIRED)
enable_testing()

add_executable(c_consumer main.c)
target_link_libraries(c_consumer PRIVATE civetweb::civetweb)
add_test(NAME c_consumer COMMAND c_consumer)

add_executable(cxx_consumer main.cc)
target_link_libraries(cxx_consumer PRIVATE civetweb::civetweb-cpp)
add_test(NAME cxx_consumer COMMAND cxx_consumer)
