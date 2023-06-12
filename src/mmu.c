#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"

uint8_t mmu_read(MMU* mmu, uint16_t addr) {
    if (addr <= 0x7fff) {
        return mmu->rom_buffer[addr];
    }
    fprintf(stderr, "UNIMPLEMENTED");
    exit(1);
}
void mmu_write(MMU* mmu, uint16_t addr, uint8_t value) {
    if (addr <= 0x7fff) {
        fprintf(stderr, "CANT WRITE ROM %u, %u\n", mmu->rom_buffer[0], value);
        exit(1);
    }
    fprintf(stderr, "UNIMPLEMENTED");
    exit(1);
}
