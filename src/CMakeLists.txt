cmake_minimum_required(VERSION 3.28.1)
project(bit_src)

IF (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
ENDIF()

#set(CMAKE_CXX_FLAGS -O3)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bit_src
        main.cpp
        position.hpp
        base.hpp
        genMove.hpp
        experiment.hpp
        evaluate.hpp
        search.hpp
        cache.hpp
)
