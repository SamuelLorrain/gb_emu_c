#ifndef MMU_H
#define MMU_H
#include <stdint.h>

typedef struct {
    uint8_t* rom_buffer;
    uint8_t wram_buffer[0x2000];
    uint8_t hram_buffer[0x80];
} MMU;

#endif

