cmake_minimum_required(VERSION 4.1)
project(Page_Array_Proyect)

set(CMAKE_CXX_STANDARD 20)

#Colocar los ejecutables del proyecto
#Generator
add_executable(generator
        src/generator/generator.cpp
        src/generator/generator.cpp)

target_link_options(generator PRIVATE -static)

#Sorter
add_executable(sorter
        src/sorter/sorter.cpp
        src/sorter/sorter.cpp
        src/sorter/PagedArray.cpp
        src/Algorithms/Algorithm1.cpp
        src/Algorithms/Algorithm2.cpp
        src/Algorithms/Algorithm3.cpp
        src/Algorithms/Algorithm4.cpp
        src/Algorithms/Algorithm5.cpp)

target_link_options(sorter PRIVATE -static)


