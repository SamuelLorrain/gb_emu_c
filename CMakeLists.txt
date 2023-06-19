cmake_minimum_required(VERSION 3.26)

project(gb_emu
    VERSION 1.0
    DESCRIPTION "a gameboy emulator"
    LANGUAGES C)

add_compile_options(-g -Wall -Wextra -Wpedantic)

find_package(SDL2 REQUIRED)

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
    gb_emu
    src/main.c
    src/file_utils.c
    src/cartridge.c
    src/debugger.c
    src/cpu.c
    src/instruction_mapper.c
    src/instruction_functions.c
    src/mmu.c
    src/stack_operations.c
    src/interrupt.c
)

target_link_libraries(gb_emu ${SDL2_LIBRARIES} m)


# tests
add_executable(
    test
    tests/call_ret_ect_instructions.c
    tests/push_pop_instructions.c
    tests/ld_16_green_instructions.c
    tests/ld_blue_instructions.c
    tests/jump_instructions.c
    tests/inc_dec_instructions.c
    tests/serial_io.c
    src/file_utils.c
    src/cartridge.c
    src/debugger.c
    src/cpu.c
    src/instruction_mapper.c
    src/instruction_functions.c
    src/mmu.c
    src/stack_operations.c
)
target_link_libraries(test criterion)
