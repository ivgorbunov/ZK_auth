cmake_minimum_required(VERSION 3.20)
project(ZK_auth)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(src)
include_directories(src/util)

set(SOURCE_FILES
    src/util/bigint.cpp)

add_compile_options(-std=c++20 -Wall -Wextra -Wpedantic -O2)

add_executable(ZK_auth ${SOURCE_FILES} main.cpp)
add_executable(ZK_auth_test ${SOURCE_FILES} tests/test.cpp)

target_link_options(ZK_auth_test PUBLIC -fsanitize=address)
target_compile_options(ZK_auth_test PUBLIC -fsanitize=address -g)

enable_testing()

find_package(GTest REQUIRED)
target_link_libraries(ZK_auth_test GTest::gtest GTest::gtest_main)

add_test(NAME test COMMAND ZK_auth_test)
