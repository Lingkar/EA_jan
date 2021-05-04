cmake_minimum_required(VERSION 3.19)
project(GOMEA_C C)

set(CMAKE_C_STANDARD 11)

add_executable(GOMEA_C
    GOMEA.c)
target_link_libraries(GOMEA_C m)
