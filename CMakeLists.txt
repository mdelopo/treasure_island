cmake_minimum_required(VERSION 3.21)
project(treasure_island C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(treasure_island
        main.c
        maputils.c
        maputlis.h)
