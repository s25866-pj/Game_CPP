cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include.SDL2)
find_package(SDL2 REQUIRED)
include_directories(${SDL_2_INCLUDE_DIRS})
add_executable(untitled main.cpp
        Player.h
        ImportIMG.h
        LoadAnimation.h
        LevelManager.h)
#target_link_libraries(untitled ${SDL2_LIBRARIES})
target_link_libraries(untitled PRIVATE SDL2 SDL2_image)