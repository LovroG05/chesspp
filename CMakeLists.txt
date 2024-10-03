cmake_minimum_required(VERSION 3.29)
project(test)

set(CMAKE_CXX_STANDARD 11)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_executable(test main.cpp)
