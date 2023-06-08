cmake_minimum_required(VERSION 3.26)

project(CMakeTutorial
    VERSION 1.0
    DESCRIPTION "a cmake tutorial"
    LANGUAGES C)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include)

add_executable(sdl2Test src/main.c src/cpu.c)
target_link_libraries(sdl2Test ${SDL2_LIBRARIES} m)
