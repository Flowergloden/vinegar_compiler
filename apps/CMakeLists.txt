# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.30)
project(vinegar_compiler)

set(CMAKE_CXX_STANDARD 20)

add_executable(vinegar_compiler
        main.cpp)

target_link_libraries(vinegar_compiler
        PUBLIC
        lexer)

# Add tasks subprojects
#include(cmake/utils.cmake)
#add_subprojects(${CMAKE_SOURCE_DIR})