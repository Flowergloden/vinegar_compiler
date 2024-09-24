# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.28..3.30)
project(vinegar_compiler)

set(CMAKE_CXX_STANDARD 20)

add_library(dfa
        SHARED
        utils/DFA.cpp
        utils/DFA.h
        utils/Token.h)

add_library(lexer
        SHARED
        Lexer.cpp
        Lexer.h)

add_library(parsing_table
        SHARED
        utils/ParsingTable.cpp
        utils/ParsingTable.h
        utils/Token.h)

add_library(bnf_tree
        SHARED
        utils/BNFTree.cpp
        utils/BNFTree.h
        utils/BitFlagSimulator.cpp
        utils/BitFlagSimulator.h)

target_link_libraries(lexer
        PUBLIC
        dfa)

target_link_libraries(parsing_table
        PUBLIC
        bnf_tree)