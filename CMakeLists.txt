cmake_minimum_required(VERSION 3.29)
project(chesspp)

set(CMAKE_CXX_STANDARD 11)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(chesspp main.cpp ${SRC_FILES})
target_include_directories(chesspp PUBLIC include/)


