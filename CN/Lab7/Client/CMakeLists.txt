cmake_minimum_required(VERSION 3.9)
project(Client)

include_directories(inc ../common)

add_subdirectory(../common common)

set(CMAKE_CXX_STANDARD 17)

add_executable(Client main.cpp Client.cpp Client.h)
find_package (Threads)

target_link_libraries (Client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Client common)
