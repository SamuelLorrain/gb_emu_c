#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <stdbool.h>
#include <stdint.h>
#include "instructions_debugger.h"

typedef struct {
    unsigned char entrypoint[4]; // jmp 0x150
    unsigned char logo[0x30];
    unsigned char title[15];
    unsigned char cgb_flag;
    unsigned char new_licence_code[0x2];
    unsigned char sgb_flag;
    unsigned char cardridge_type;
    unsigned char rom_size;
    unsigned char ram_size;
    unsigned char destination_code;
    unsigned char old_licence_code;
    unsigned char mask_rom_version_number;
    unsigned char header_checksum;
    unsigned char global_checksum[0x2]; // big endian
} RomHeader;

void parse_rom_header(RomHeader* rom_header, unsigned char const buffer[static 0x150]);
void debug_header(RomHeader* rom_header);

#endif
