cmake_minimum_required(VERSION 3.5.1)
project(linux_ws_server)

set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
find_package(civetweb)


include_directories(
    ${civetweb_INCLUDE_DIR}
)

link_directories (
    /usr/local/lib
)

add_executable(${TARGET_NAME} main.cc)

target_link_libraries(${TARGET_NAME}
    libcivetweb-cpp.so
    libcivetweb.so
)
