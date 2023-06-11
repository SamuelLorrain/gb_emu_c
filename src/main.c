#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "debugger.h"
#include "cpu.h"
#include "mmu.h"

size_t get_file_size(FILE* const fp) {
    size_t current_position = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, current_position);
    return file_size;
}


int main(int argc, char* argv[argc + 1]) {
    if (argc < 2) {
        fputs("First argument must be the rom file\n", stderr);
        return EXIT_FAILURE;
    }
    FILE* rom_file = fopen(argv[1], "rb");
    size_t file_size = get_file_size(rom_file);

    unsigned char rom_buffer[file_size];
    size_t fread_result = fread(rom_buffer, 1, file_size, rom_file);
    fclose(rom_file);
    if (fread_result != file_size) {
        fputs("Unable to read rom file\n", stderr);
        return EXIT_FAILURE;
    }

    // debug rom header
    /* RomHeader rom_header = {0}; */
    /* parse_rom_header(&rom_header, rom_buffer); */
    /* debug_header(&rom_header); */

    // Begin
    Registers regs = { .pc = 0x100, .sp = 0xfffe };
    unsigned char ram[0xffff] = {0};
    load_rom(ram, rom_buffer);
    /* for(int i = 0 ; i < 50; i++) { */
    /*     execute_next(&regs, ram); */
    /* } */
    for(int i = 0; i <= 0xff; i++) {
        ram[i] = i;
    }
    for(int i =  0; i <= 0xff; i++) {
        regs.pc = i;
        execute_next(&regs, ram);
    }
    return EXIT_SUCCESS;
}

