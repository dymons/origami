cmake_minimum_required(VERSION 3.12...3.14)
project(origami_lexical)

file(GLOB ORIGAMI_LEXICAL_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${ORIGAMI_LEXICAL_SRC})
add_library(origami::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)