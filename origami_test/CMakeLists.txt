cmake_minimum_required(VERSION 2.8)
project(origami_test)

find_package(Catch2 REQUIRED)

add_executable(origami_test main_test.cpp operators_test.cpp)
target_link_libraries(origami_test Catch2::Catch2 origami::origami_lexical origami::origami_syntax)
