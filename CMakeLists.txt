cmake_minimum_required(VERSION 3.9)
project(fixed_vector)

set(CMAKE_CXX_STANDARD 14)

add_executable(fixed_vector fixed_vector.h tests.cpp)