cmake_minimum_required(VERSION 3.16)
project(simple_map C)

set(CMAKE_C_STANDARD 99)

add_executable(simple_map test.c simple_map.h simple_map.min.h)
