#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>

typedef struct {
    uint8_t entrypoint[4]; // usually jmp 0x150
    uint8_t logo[0x30];
    uint8_t title[15];
    uint8_t cgb_flag;
    uint16_t new_licence_code;
    uint8_t sgb_flag;
    uint8_t cartridge_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t destination_code;
    uint8_t old_licence_code;
    uint8_t mask_rom_version_number;
    uint8_t header_checksum;
    uint16_t global_checksum; // big endian
} RomHeader;


void parse_rom_header(RomHeader* rom_header, uint8_t const buffer[static 0x150]);
const char* get_rom_cartridge_type(RomHeader* const rom_header);
bool is_header_checksum_valid(
        RomHeader* rom_header,
        uint8_t const rom_buffer[static 0x14C]
    );

#endif
