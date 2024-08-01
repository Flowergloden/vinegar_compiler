# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.28..3.30)
project(vinegar_compiler)

set(CMAKE_CXX_STANDARD 20)

add_executable(unit_test
        UnitTest.cpp)

target_link_libraries(unit_test
        parsing_table)

target_include_directories(unit_test
        PUBLIC
        ../src)