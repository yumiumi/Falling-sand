cmake_minimum_required(VERSION 3.28)
project(Falling_sand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Falling_sand main.cpp "types.hpp" imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp rlImGui/rlImGui.cpp)

target_link_libraries(Falling_sand raylib winmm)
target_include_directories(Falling_sand PRIVATE SYSTEM raylib-5.5_win64_msvc16/include imgui rlImGui)
target_link_directories(Falling_sand PRIVATE raylib-5.5_win64_msvc16/lib)
