cmake_minimum_required(VERSION 3.10)
project(common)

find_package(Threads)

set(CMAKE_CXX_STANDARD 17)

add_library(common message.cpp message.h socket.cpp socket.h)

add_executable(Test test.cpp)
target_link_libraries(Test common ${CMAKE_THREAD_LIBS_INIT})
