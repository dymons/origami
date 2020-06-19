cmake_minimum_required(VERSION 2.8)
project(origami_syntax)

file(GLOB ORIGAMI_LEXICAL_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${ORIGAMI_LEXICAL_SRC})
add_library(origami::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC origami::origami_lexical)