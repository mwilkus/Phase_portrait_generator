cmake_minimum_required(VERSION 3.15)
project(Phase_poitrat_generator)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/assests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/control)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/cordinats)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/numeric)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/rendering)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/input_eq)

find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} rendering control input_eq numeric SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

