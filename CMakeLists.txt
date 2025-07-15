cmake_minimum_required(VERSION 3.28)
project(Falling_sand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Falling_sand main.cpp "types.hpp")
target_link_libraries(Falling_sand raylib winmm)
target_include_directories(Falling_sand PRIVATE SYSTEM raylib-5.0_win64_msvc16/include)
target_link_directories(Falling_sand PRIVATE raylib-5.0_win64_msvc16/lib)
