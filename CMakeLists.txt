cmake_minimum_required(VERSION 3.28..3.30)
project(vinegar_compiler)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(apps)
add_subdirectory(src)
add_subdirectory(tests)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/dfa.txt
        ${CMAKE_CURRENT_BINARY_DIR}/config/dfa.txt COPYONLY)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code.vml
            ${CMAKE_CURRENT_BINARY_DIR}/code.vml COPYONLY)
endif ()