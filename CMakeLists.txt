cmake_minimum_required(VERSION 3.17)
project(explorer)

set(CMAKE_CXX_STANDARD 11)

add_executable(explorer main.cpp explorer.cpp explorer.h)
target_compile_options(explorer PRIVATE -ffast-math -O3)
target_link_libraries(explorer sfml-graphics sfml-system sfml-window sfml-audio)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(explorer OpenMP::OpenMP_CXX)
endif()