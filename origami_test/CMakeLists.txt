cmake_minimum_required(VERSION 3.5.1)
project(origami_test)

find_package(Catch2 REQUIRED)

file(GLOB ORIGAMI_TEST_SRC ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(origami_test ${ORIGAMI_TEST_SRC})
target_link_libraries(origami_test Catch2::Catch2 origami::origami_lexical origami::origami_syntax)