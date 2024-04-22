set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.10)

project(battleship)

include_directories(include)

add_executable(battleship src/battleship.c src/battleship_tools.c src/file_stuff.c)
