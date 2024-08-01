cmake_minimum_required(VERSION 3.28)
project(bit_src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bit_src
        bitboard.cpp
        bitboard.hpp)
