#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"

Test(jump_instructions, _0x20_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x20,
        0x10,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x012);

    cpu.regs.pc = 0;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x02);

    // negative address
    cpu.regs.pc = 0;
    cpu.regs.f_z = 0;
    ram[0x0001] = -2;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0);
}

Test(jump_instructions, _0x30_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x30,
        0x10,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x012);

    cpu.regs.pc = 0;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x02);

    // negative address
    cpu.regs.pc = 0;
    cpu.regs.f_c = 0;
    ram[0x0001] = -2;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0);
}


Test(jump_instructions, _0x18_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x18,
        0x10,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x012);

    // negative address
    cpu.regs.pc = 0;
    ram[0x0001] = -2;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0);
}


Test(jump_instructions, _0x28_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x28,
        0x10,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x012);

    cpu.regs.pc = 0;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x02);

    // negative address
    cpu.regs.pc = 0;
    cpu.regs.f_z = 1;
    ram[0x0001] = -2;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0);
}

Test(jump_instructions, _0x38_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x38,
        0x10,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x012);

    cpu.regs.pc = 0;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x02);

    // negative address
    cpu.regs.pc = 0;
    cpu.regs.f_c = 1;
    ram[0x0001] = -2;
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0);
}

Test(jump_instructions, _0xc2_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xc2,
        0x90,
        0x50,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x5090);

    cpu.regs.pc = 0;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x03);
}

Test(jump_instructions, _0xd2_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xd2,
        0x90,
        0x50,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x5090);

    cpu.regs.pc = 0;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x03);
}


Test(jump_instructions, _0xca_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xca,
        0x90,
        0x50,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x5090);

    cpu.regs.pc = 0;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x03);
}

Test(jump_instructions, _0xda_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xda,
        0x90,
        0x50,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x5090);

    cpu.regs.pc = 0;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x03);
}

Test(jump_instructions, _0xc3_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xc3,
        0x34,
        0x12,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x1234);
}

Test(jump_instructions, _0xe9_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xe9,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.hl = 0x6789;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x6789);
}
