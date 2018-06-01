cmake_minimum_required(VERSION 3.9)
project(Server)

include_directories(inc ../common)

add_subdirectory(../common common)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server main.cpp Server.cpp Server.h)
find_package (Threads)

target_link_libraries (Server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Server common)
