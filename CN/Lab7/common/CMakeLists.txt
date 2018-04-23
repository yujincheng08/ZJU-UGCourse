cmake_minimum_required(VERSION 3.10)
project(common)

find_package(Threads)


FIND_PACKAGE(Boost COMPONENTS system)


IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF (Boost_FOUND)


set(CMAKE_CXX_STANDARD 17)

add_library(common message.cpp message.h)

add_executable(Test test.cpp)
target_link_libraries(Test common ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
