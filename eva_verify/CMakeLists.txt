cmake_minimum_required(VERSION 3.26)
project(Ways49)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_CXX_FLAGS -O3)

include_directories(.)

add_executable(Ways49
        base.h
        base2.h
        book.cpp
        book.h
        eleeye.cpp
        evaluate.cpp
        genmoves.cpp
        hash.cpp
        hash.h
        movesort.cpp
        movesort.h
        parse.h
        pipe.cpp
        pipe.h
        position.cpp
        position.h
        preeval.cpp
        preeval.h
        pregen.cpp
        pregen.h
        rc4prng.h
        search.cpp
        search.h
        ucci.cpp
        ucci.h)
