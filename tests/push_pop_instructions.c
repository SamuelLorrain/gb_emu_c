#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"

Test(push_pop_instructions, _0xc1_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xc1,
        [0x1000] = 0x34,
        [0x1001] = 0x12
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x1000;
    cpu.regs.bc = 0xaaaa;
    step(&cpu);
    cr_assert(cpu.regs.bc == 0x1234);
    cr_assert(cpu.regs.sp == 0x1002);
}

Test(push_pop_instructions, _0xd1_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xd1,
        [0x7000] = 0xcd,
        [0x7001] = 0xab
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x7000;
    cpu.regs.de = 0xaaaa;
    step(&cpu);
    cr_assert(cpu.regs.de == 0xabcd);
    cr_assert(cpu.regs.sp == 0x7002);
}

Test(push_pop_instructions, _0xe1_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xe1,
        [0x5000] = 0x89,
        [0x5001] = 0x67
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5000;
    cpu.regs.hl = 0xaaaa;
    step(&cpu);
    cr_assert(cpu.regs.hl == 0x6789);
    cr_assert(cpu.regs.sp == 0x5002);
}

Test(push_pop_instructions, _0xf1_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xf1,
        [0x5050] = 0xde,
        [0x5051] = 0xf0
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5050;
    cpu.regs.af = 0xaaaa;
    step(&cpu);
    // Special case for AF, may
    // need to review this
    cr_assert(cpu.regs.af == 0xf0d0);
    cr_assert(cpu.regs.sp == 0x5052);
}

Test(push_pop_instructions, _0xc5_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xc5,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5000;
    cpu.regs.bc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x4ffe);
    cr_assert(ram[0x4fff] == 0x12);
    cr_assert(ram[0x4ffe] == 0x34);
}

Test(push_pop_instructions, _0xd5_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xd5,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5000;
    cpu.regs.de = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x4ffe);
    cr_assert(ram[0x4fff] == 0x12);
    cr_assert(ram[0x4ffe] == 0x34);
}

Test(push_pop_instructions, _0xe5_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xe5,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5000;
    cpu.regs.hl = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x4ffe);
    cr_assert(ram[0x4fff] == 0x12);
    cr_assert(ram[0x4ffe] == 0x34);
}

Test(push_pop_instructions, _0xf5_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xf5,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5000;
    cpu.regs.af = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x4ffe);
    cr_assert(ram[0x4fff] == 0x12);
    cr_assert(ram[0x4ffe] == 0x34);
}

Test(push_pop_instructions, push_then_pop_bc) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xc5,    // PUSH BC
        0x01,    // LOAD BC, 0x0000
        0x00,
        0x00,
        0xc1,   // POP BC
        [0x5000] = 0x0,
        [0x5001] = 0x0,
        [0x5002] = 0x0,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0x5003;
    cpu.regs.bc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x5001);
    cr_assert(ram[0x5002] == 0x12);
    cr_assert(ram[0x5001] == 0x34);
    step(&cpu);
    cr_assert(cpu.regs.bc == 0x0000);
    step(&cpu);
    cr_assert(cpu.regs.sp == 0x5003);
    cr_assert(cpu.regs.bc == 0x1234);
}
