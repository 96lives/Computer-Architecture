cmake_minimum_required(VERSION 3.9)
enable_language(C ASM)
project(hw3 C)

set(CMAKE_C_STANDARD 99)

add_executable(hw3
        bmp.c
        bmp.h
        bmpgrid.s
        )