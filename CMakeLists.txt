cmake_minimum_required(VERSION 3.12...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(origami VERSION 0.0.1 DESCRIPTION "C++20 Static Code Analysis" LANGUAGES CXX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG=ON -D_GLIBCXX_DEBUG=ON -fsanitize=address --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuseless-cast -Wdouble-promotion -Wformat=2 -Weffc++")
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(origami_lexical)
add_subdirectory(origami_syntax)
add_subdirectory(origami_test)