cmake_minimum_required(VERSION 3.16)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SnakeGame
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Food.cpp
    src/Renderer.cpp
    src/Input.cpp
    src/SaveSystem.cpp
)

if (WIN32)
    target_link_libraries(SnakeGame PRIVATE winmm)
endif()
