cmake_minimum_required(VERSION 3.1)
project(Task1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE "Debug")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    # show all warnings and optimise for debugging, allow profiling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -O0 -UNDEBUG")
    set(CMAKE_BUILD_TYPE "Debug")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    # just optimize as much as possible
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(SOURCES
    big_num.h
    big_num.cpp
)

add_executable(tests tests.cpp ${SOURCES})
# Add compile options for the tests
target_compile_definitions(tests PRIVATE TEST_BUILD)

add_executable(task1 main.cpp ${SOURCES})

