cmake_minimum_required(VERSION 3.2)

#CXX Setup
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_COMPILER /usr/bin/gcc-7)
#set(CMAKE_C_COMPILER /usr/bin/clang-7)
#set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++-8)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-7)  #clang++-8 NOT available in ubuntu 15 apr 2019

#set(GCC_EXTRA_COMPILE_FLAGS "-Ofast -fopenmp")
set(GCC_EXTRA_COMPILE_FLAGS "-O0 -g -fopenmp")
#set(GCC_EXTRA_LINK_FLAGS    "")


#reset existing
set(CMAKE_CXX_FLAGS  ${GCC_EXTRA_COMPILE_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS  ${GCC_EXTRA_LINK_FLAGS})

#Project name
project(stopwatch)


#include folders
set(INCLUDE "/home/hdaniel/Dropbox/01-libs/cpp")
include_directories(. ${INCLUDE} )

#Unit tests
set(UNITTEST "tests")
add_executable(${UNITTEST} ${UNITTEST}.cpp)


