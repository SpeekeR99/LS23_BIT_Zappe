cmake_minimum_required(VERSION 3.24)
project(prj)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    main
    main.cpp
    sha256.h
    biginteger.cpp
    biginteger.h
    bigintegerversion.cpp
    bigintegerversion.h
    bigintegerexception.h
)
