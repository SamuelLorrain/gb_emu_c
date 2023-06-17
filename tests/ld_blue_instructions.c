#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"

// MAY NEED TESTING FOR INSTRUCTION 0x40 to 0x7f (simplest ones)
// CAN'T TEST 0xe0, 0xe2, 0xf0 and 0xf2 because we don't have 0xff__ read/write yet

Test(ld_blue_instructions, _0x02_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x02,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.a = 0x99;
    cpu.regs.bc = 0x1234;
    step(&cpu);
    cr_assert(ram[0x1234] == 0x99);
}

Test(ld_blue_instructions, _0x12_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x12,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.a = 0x99;
    cpu.regs.de = 0x1234;
    step(&cpu);
    cr_assert(ram[0x1234] == 0x99);
}

Test(ld_blue_instructions, _0x22_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x22,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.a = 0x99;
    cpu.regs.hl = 0x1234;
    step(&cpu);
    cr_assert(ram[0x1234] == 0x99);
    cr_assert(cpu.regs.hl == 0x1235);
}

Test(ld_blue_instructions, _0x32_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x32,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.a = 0x99;
    cpu.regs.hl = 0x1234;
    step(&cpu);
    cr_assert(ram[0x1234] == 0x99);
    cr_assert(cpu.regs.hl == 0x1233);
}

Test(ld_blue_instructions, _0x06_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x06,
        0x55
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.b = 0x99;
    step(&cpu);
    cr_assert(cpu.regs.b == 0x55);
}

Test(ld_blue_instructions, _0x16_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x16,
        0x55
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.d = 0x99;
    step(&cpu);
    cr_assert(cpu.regs.d == 0x55);
}

Test(ld_blue_instructions, _0x26_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x26,
        0x55
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.h = 0x99;
    step(&cpu);
    cr_assert(cpu.regs.h == 0x55);
}

Test(ld_blue_instructions, _0x36_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x36,
        0x55
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.hl = 0x0200;
    step(&cpu);
    cr_assert(ram[cpu.regs.hl] == 0x55);
}

Test(ld_blue_instructions, _0x0a_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x0a,
        [0x200] = 0x89
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.bc = 0x0200;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x89);
}

Test(ld_blue_instructions, _0x1a_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x1a,
        [0x200] = 0x89
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.de = 0x0200;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x89);
}

Test(ld_blue_instructions, _0x2a_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x2a,
        [0x200] = 0x89
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.hl = 0x0200;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x89);
    cr_assert(cpu.regs.hl == 0x0201);
}

Test(ld_blue_instructions, _0x3a_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x3a,
        [0x200] = 0x89
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.hl = 0x0200;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x89);
    cr_assert(cpu.regs.hl == 0x01ff);
}

Test(ld_blue_instructions, _0x0e_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x0e,
        0x3f,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.c == 0x3f);
}

Test(ld_blue_instructions, _0x1e_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x1e,
        0x3f,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.e == 0x3f);
}

Test(ld_blue_instructions, _0x2e_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x2e,
        0x3f,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.l == 0x3f);
}

Test(ld_blue_instructions, _0x3e_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0x3e,
        0x3f,
    };
    cpu.mmu.rom_buffer = ram;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x3f);
}

/* Test(ld_blue_instructions, _0xe0_instruction) { */
/*     Cpu cpu = {0}; */
/*     unsigned char ram[0xffff] = { */
/*         0xe0, */
/*         0x3f, */
/*     }; */
/*     cpu.mmu.rom_buffer = ram; */
/*     cpu.regs.a = 0x59; */
/*     step(&cpu); */
/*     cr_assert(ram[0xff3f] == cpu.regs.a); */
/* } */

Test(ld_blue_instructions, _0xea_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xea,
        0x3f,
        0x41,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.a = 0x99;
    step(&cpu);
    cr_assert(ram[0x413f] == 0x99);
}

Test(ld_blue_instructions, _0xfa_instruction) {
    Cpu cpu = {0};
    unsigned char ram[0xffff] = {
        0xfa,
        0x3f,
        0x41,
        [0x413f] = 0x99,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.a = 0xaa;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x99);
}
