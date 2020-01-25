cmake_minimum_required(VERSION 3.10)

# активация стандарта C++17
set(CMAKE_CXX_STANDARD 17)

add_definitions(/MP) # активация многопоточной сборки
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}") # активация использования OpenMP

project(test)

add_executable(test main.cpp)
