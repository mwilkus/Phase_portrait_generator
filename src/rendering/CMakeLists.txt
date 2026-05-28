cmake_minimum_required(VERSION 3.15)
project(rendering)

find_package(SDL2 REQUIRED)

add_library(rendering SHARED
    rendering.cpp
)

target_link_libraries(rendering PUBLIC SDL2::SDL2 numeric input_eq)

target_include_directories(rendering INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)