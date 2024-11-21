cmake_minimum_required(VERSION 3.29)
project(PC_UF3)

set(CMAKE_CXX_STANDARD 20)

add_executable(PC_UF3 main.cpp
        Dynarray.h
        Vector.h
)
