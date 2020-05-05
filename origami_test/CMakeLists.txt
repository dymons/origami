cmake_minimum_required(VERSION 2.8)
project(origami_test)

if(NOT GTEST_FOUND)
    IF(IS_DIRECTORY "/usr/src/gtest/")
        message(STATUS "Found google test sources in /usr/src/gtest/")

        add_subdirectory(/usr/src/gtest/ ${CMAKE_BINARY_DIR}/gtest)

        set(GTEST_FOUND TRUE)
        set(GTEST_BOTH_LIBRARIES gtest gtest_main)
        set(GTEST_LIBRARIES gtest)
        set(GTEST_MAIN_LIBRARIES gtest_main)
    ELSE()
        find_package(GTest)
    ENDIF()
endif()

if(GTEST_FOUND)
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

add_executable(character_sets_test character_sets_test.cpp)
target_link_libraries(character_sets_test gtest origami::origami_lexical)
add_test(NAME character_sets_test COMMAND character_sets_test)

add_executable(tokens_test tokens_test.cpp)
target_link_libraries(tokens_test gtest origami::origami_lexical)
add_test(NAME tokens_test COMMAND tokens_test)