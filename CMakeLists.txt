cmake_minimum_required(VERSION 3.26)

project(gb_emulator
    VERSION 1.0
    DESCRIPTION "a gameboy emulator"
    LANGUAGES C)

add_compile_options(-Wall -Wextra -Wpedantic)

# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})
# target_link_libraries(debugger ${SDL2_LIBRARIES} m)

include_directories(include)

add_executable(debugger src/main.c src/cpu.c src/debugger.c src/mmu.c)

# tests
add_executable(test tests/test.c src/cpu.c)
target_link_libraries(test criterion)
