cmake_minimum_required(VERSION 3.17)
project(covid)

set(CMAKE_CXX_STANDARD 14)

add_executable(covid2 main2.cpp records.cpp records.h utils.h)