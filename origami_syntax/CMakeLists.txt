cmake_minimum_required(VERSION 3.12...3.14)
project(origami_syntax)

find_package(fmt REQUIRED)

file(GLOB ORIGAMI_SYNTAX_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${ORIGAMI_SYNTAX_SRC})
add_library(origami::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC origami::origami_lexical fmt::fmt)