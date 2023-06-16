#include <criterion/criterion.h>
#include "cpu.h"


Test(cpu_instructions, ld_16bits_registers) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x02,
    };
    cpu.regs.a = 0x10;
    cpu.regs.bc = 0x1234;
    cpu.mmu.rom_buffer = ram;

    step(&cpu);
    cr_assert(ram[0x1234] == 0x10);
}

