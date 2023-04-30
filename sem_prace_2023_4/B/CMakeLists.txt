cmake_minimum_required(VERSION 3.22)
project(prj)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(
        main
        main.cpp
        biginteger.cpp
        biginteger.h
        bigintegerexception.h
        bigintegerversion.cpp
        bigintegerversion.h
)
