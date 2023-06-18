#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "debugger.h"
#include "cpu.h"
/* #include "SDL.h" */
#include "file_utils.h"
#include "cartridge.h"
#include "debugger.h"
#include "instruction_functions.h"
#include "interrupt.h"

int main(int argc, char* argv[argc + 1]) {
    /* if (argc < 2) { */
    /*     fprintf(stderr, "Should give rom at first argument\n"); */
    /*     exit(1); */
    /* } */

    /* if(SDL_Init(SDL_INIT_VIDEO) < 0) { */
    /*     fprintf(stderr, "SDL error : %s", SDL_GetError()); */
    /*     exit(1); */
    /* } */
    /* SDL_Quit(); */

    FILE* rom = fopen(argv[1], "rb");
    if(!rom) {
        fprintf(stderr, "Should give rom at first argument\n");
        exit(1);
    }

    size_t rom_size = get_file_size(rom);
    uint8_t rom_buffer[rom_size];
    fread(rom_buffer, rom_size, 1, rom);
    fclose(rom);
    RomHeader rom_header = {0};
    parse_rom_header(&rom_header, rom_buffer);
    debug_header(&rom_header, rom_buffer);

    Cpu cpu = {0};
    cpu.regs.pc = 0x100;
    cpu.mmu.rom_buffer = rom_buffer;

    for(int i = 0; i < 100; i++) {
        step(&cpu);
        reset_instruction_state(&cpu);

        if (cpu.halted) {
            // pause, not implemented (need to make thread to sleep)
            cpu.halted = false;
        }
        if (cpu.interruption_master_enable) {
            handle_interrupt(&cpu);
            cpu.enabling_ime = false;
        }
        if (cpu.enabling_ime) {
            cpu.interruption_master_enable = true;
            cpu.enabling_ime = false;
        }

    }
    return EXIT_SUCCESS;
}

