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
        fprintf(stderr, "Unable to load rom\n");
        exit(1);
    }

    size_t rom_size = get_file_size(rom);
    uint8_t rom_buffer[rom_size];

    fread(rom_buffer, rom_size, 1, rom);

    RomHeader rom_header = {0};
    parse_rom_header(&rom_header, rom_buffer);
    debug_header(&rom_header, rom_buffer);
    fclose(rom);
    return EXIT_SUCCESS;
}

