cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG=ON)
endif()

project(origami VERSION 1.0.0 DESCRIPTION "C++20 Module Example" LANGUAGES CXX)

add_subdirectory(origami_lexical)
add_subdirectory(origami_syntax)
add_subdirectory(origami_test)