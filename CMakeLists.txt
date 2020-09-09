cmake_minimum_required(VERSION 3.17)
project(nahfpa C)

set(CMAKE_C_STANDARD 99)

add_executable(nahfpa main.c data_structures/List.c data_structures/List.h)