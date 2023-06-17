#ifndef MMU_H
#define MMU_H
#include "cpu.h"

uint8_t mmu_read(Cpu* cpu, uint16_t addr);
uint16_t mmu_read16(Cpu* cpu, uint16_t addr);
void mmu_write(Cpu* cpu, uint16_t addr, uint8_t value);
void mmu_write16(Cpu* cpu, uint16_t addr, uint16_t value);

#endif
