#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"


Test(cpu_instructions, _0x01_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x01,
        0x34,
        0x12,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.bc == 0x1234);
}

Test(cpu_instructions, _0x11_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x11,
        0x34,
        0x12,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.de == 0x1234);
}

Test(cpu_instructions, _0x21_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x21,
        0x34,
        0x12,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.hl == 0x1234);
}

Test(cpu_instructions, _0x31_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x31,
        0x34,
        0x12,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x1234);
}

Test(cpu_instructions, _0x08_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x08,
        0x78,
        0x56,
    };
    cpu.regs.sp = 0x1234;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(ram[0x5678] == 0x34);
    cr_assert(ram[0x5679] == 0x12);
}

Test(cpu_instructions, _0xf9_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xf9
    };
    cpu.regs.sp = 0x1234;
    cpu.regs.hl = 0xabcd;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.sp == cpu.regs.hl);
}

Test(cpu_instructions, _0xf8_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xf8
    };
    cpu.regs.sp = 0x1234;
    cpu.regs.hl = 0xabcd;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.sp == cpu.regs.hl);
}

