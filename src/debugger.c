#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debugger.h"

void debug_header(RomHeader* rom_header) {
    putchar('\n');
    fputs("logo: ", stdout);
    for(int i = 0; i < 0x30; i++) {
        printf("%x ", rom_header->logo[i]);
    }
    putchar('\n');
    fputs("title: ", stdout);
    for(int i = 0; i < 15; i++) {
        if (rom_header->title[i] == 0) {
            break;
        }
        putchar(rom_header->title[i]);
    }
    putchar('\n');
    printf("cgb_flag: 0x%02x\n", rom_header->cgb_flag);
    printf("new_licence_code: 0x%02x 0x%02x\n", rom_header->new_licence_code[0], rom_header->new_licence_code[1]);
    printf("sgb_flag: 0x%02x\n", rom_header->sgb_flag);
    printf("cardridge_type: 0x%02x\n", rom_header->cardridge_type);
    printf("rom_size: 0x%02x\n", rom_header->rom_size);
    printf("ram_size: 0x%02x\n", rom_header->ram_size);
    printf("destination_code: 0x%02x\n", rom_header->destination_code);
    printf("old_licence_code: 0x%02x\n", rom_header->old_licence_code);
    printf("mask_rom_version_number: 0x%02x\n", rom_header->mask_rom_version_number);
    printf("header_checksum: 0x%02x\n", rom_header->header_checksum);
    printf("global_checksum: 0x%2x%2x\n", rom_header->global_checksum[0], rom_header->global_checksum[1]); // big endian
}

void parse_rom_header(RomHeader* rom_header, unsigned char const buffer[static 0x150]) {
    memcpy(rom_header, &(buffer[0x100]), 0x50);
}
