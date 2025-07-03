cmake_minimum_required(VERSION 3.28)
project(Falling_sand)

add_executable(Falling_sand main.cpp)
target_link_libraries(Falling_sand raylib winmm)
target_include_directories(Falling_sand PRIVATE SYSTEM raylib-5.0_win64_msvc16/include)
target_link_directories(Falling_sand PRIVATE raylib-5.0_win64_msvc16/lib)