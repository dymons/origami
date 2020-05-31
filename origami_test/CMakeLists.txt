cmake_minimum_required(VERSION 2.8)
project(origami_test)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(origami_test main_test.cpp operators_test.cpp preprocessor_test.cpp)
target_link_libraries(origami_test ${GTEST_LIBRARIES} pthread origami::origami_lexical)
