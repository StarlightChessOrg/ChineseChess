cmake_minimum_required(VERSION 3.28.1)
project(bit_src)

IF (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
ENDIF()

set(CMAKE_CXX_FLAGS -O3)
set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_executable(bit_src
        main.cpp
        base.hpp
        experiment.hpp
        preGenMove.hpp
        genMove.hpp
        position.hpp
        status.hpp
        cache.hpp
        nnue.hpp
        search.hpp
        pipe.hpp
)

FIND_PACKAGE(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()
