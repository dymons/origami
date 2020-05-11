cmake_minimum_required(VERSION 2.8)
project(origami_lexical)

add_library(${PROJECT_NAME} src/character_sets.cpp src/lexical_conventions.cpp)
add_library(origami::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)