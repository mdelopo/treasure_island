cmake_minimum_required(VERSION 3.21)
project(treasure_island C)

set(CMAKE_C_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH "C:\\Users\\Manolis\\Documents\\AUTH THMMY\\1ST SEMESTER\\STRUCTURED PROGRAMMING\\treasure_island")
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(.)

add_executable(treasure_island
        main.c
        maputils.c
        maputlis.h)
target_link_libraries(treasure_island ${CURSES_LIBRARIES})
