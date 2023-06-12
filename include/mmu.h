#ifndef MMU_H
#define MMU_H
#include <stdint.h>

typedef struct {
    uint8_t* rom_buffer;
} MMU;

uint8_t mmu_read(MMU* mmu, uint16_t addr);
void mmu_write(MMU* mmu, uint16_t addr, uint8_t value);

#endif

