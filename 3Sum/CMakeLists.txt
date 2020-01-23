cmake_minimum_required(VERSION 2.4)

# активация многопоточной сборки
add_definitions(/MP)
# активация стандарта C++17
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

project(test)

add_executable(test main.cpp)
