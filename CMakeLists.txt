cmake_minimum_required(VERSION 3.17)
project(covid)

set(CMAKE_CXX_STANDARD 17)

add_executable(covid main.cpp utils.h field.cpp field.h data_frame.cpp data_frame.h operator.cpp operator.h)