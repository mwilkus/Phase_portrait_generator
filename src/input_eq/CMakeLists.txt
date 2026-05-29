cmake_minimum_required(VERSION 3.15)
project(input_eq)

add_library(input_eq SHARED input_eq.cpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(input_eq PRIVATE control numeric parser cordinats SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

target_include_directories(input_eq PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)