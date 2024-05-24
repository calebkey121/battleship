cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)

project(6_x)

include_directories(include)

# add_executable(6_3 src/6_3.c)
# target_compile_options(6_3 PRIVATE -mlong-double-128)

#add_executable(6_4 src/6_4.c)

add_executable(battleship src/battleship.c src/battleship_tools.c src/file_stuff.c src/board.c src/boat.c)

#add_executable(6_6 src/6_6.c)
