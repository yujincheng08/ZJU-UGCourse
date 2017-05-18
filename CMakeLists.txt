cmake_minimum_required(VERSION 3.7)
project(MiniSQL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp test_create_table.c test.h miniSqlDef.h logutils.c parse.h parse.c)

add_executable(MiniSQL ${SOURCE_FILES})