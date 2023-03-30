cmake_minimum_required(VERSION 3.23)
project(rg_projekat)

set(CMAKE_CXX_STANDARD 14)

find_package(glfw3 REQUIRED)

find_package(OpenGL REQUIRED)

add_subdirectory(glad/)

add_executable(rg_projekat main.cpp include/Shader.h include/Camera.h)

target_link_libraries(rg_projekat glad glfw OpenGL::GL pthread X11 Xrandr Xi dl)