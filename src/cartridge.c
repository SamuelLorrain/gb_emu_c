#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "cartridge.h"


void parse_rom_header(RomHeader* const rom_header, uint8_t const rom_buffer[static 0x150]) {
    memcpy(rom_header, &(rom_buffer[0x100]), 0x50);
}

bool is_header_checksum_valid(RomHeader* rom_header, uint8_t const rom_buffer[static 0x14C]) {
    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - rom_buffer[address] - 1;
    }
    printf("COMPUTED CHECKSUM : 0x%x\n", checksum);
    return (checksum == rom_header->header_checksum);
}
