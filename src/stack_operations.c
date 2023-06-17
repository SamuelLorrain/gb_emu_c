#include <stdint.h>
#include "cpu.h"
#include "mmu.h"
#include "stack_operations.h"


void stack_push(Cpu* cpu, uint8_t value) {
    cpu->regs.sp--;
    mmu_write(cpu, cpu->regs.sp, value);
}

void stack_push16(Cpu* cpu, uint16_t value) {
    stack_push(cpu, (value >> 8) & 0xff);
    stack_push(cpu, value & 0xff);
}

uint8_t stack_pop(Cpu* cpu) {
    uint8_t value = mmu_read(cpu, cpu->regs.sp);
    cpu->regs.sp++;
    return value;
}

uint16_t stack_pop16(Cpu* cpu) {
    uint16_t hi = stack_pop(cpu);
    uint16_t lo = stack_pop(cpu);
    return (hi << 8) | lo;
}
