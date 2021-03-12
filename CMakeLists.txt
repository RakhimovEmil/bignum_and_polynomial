cmake_minimum_required(VERSION 3.17)
project(HW_2)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined,address")

add_executable(HW_2 bignum.hpp polynomial.hpp smoke_test.cpp)
