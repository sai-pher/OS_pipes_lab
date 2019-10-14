cmake_minimum_required(VERSION 3.14)
project(lab_4 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab_4 main.c)
target_link_libraries(lab_4 m)

add_executable(PilesExample PipesExample.c)
