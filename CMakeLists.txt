cmake_minimum_required(VERSION 3.30)
project(vinegar_compiler)

set(CMAKE_CXX_STANDARD 20)

add_executable(vinegar_compiler main.cpp
        Lexer.cpp
        Lexer.h)
