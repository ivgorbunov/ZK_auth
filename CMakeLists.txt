cmake_minimum_required(VERSION 3.28)
project(ZK_auth)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(src/util)

add_executable(ZK_auth main.cpp)
