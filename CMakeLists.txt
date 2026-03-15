cmake_minimum_required(VERSION 4.1)
project(Page_Array_Proyect)

set(CMAKE_CXX_STANDARD 20)

#Colocar los ejecutables del proyecto
add_executable(generator
        src/generator/generator.cpp
        src/generator/generator.cpp)

add_executable(sorter
        src/sorter/sorter.cpp
        src/sorter/sorter.cpp)


