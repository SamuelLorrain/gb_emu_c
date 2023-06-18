#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"

Test(inc_dec_instructions, _0x03_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x03,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.bc = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.bc == 0);

    cpu.regs.pc = 0x1234;
    cpu.regs.bc = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.bc == 1);
}

Test(inc_dec_instructions, _0x13_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x13,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.de = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.de == 0);

    cpu.regs.pc = 0x1234;
    cpu.regs.de = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.de == 1);
}

Test(inc_dec_instructions, _0x23_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x23,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.hl == 0);

    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.hl == 1);
}

Test(inc_dec_instructions, _0x33_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x33,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.sp == 0);

    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.sp == 1);
}


Test(inc_dec_instructions, _0x0b_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x0b,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.bc = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.bc == 0xfffe);

    cpu.regs.pc = 0x1234;
    cpu.regs.bc = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.bc == 0xffff);
}

Test(inc_dec_instructions, _0x1b_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x1b,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.de = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.de == 0xfffe);

    cpu.regs.pc = 0x1234;
    cpu.regs.de = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.de == 0xffff);
}

Test(inc_dec_instructions, _0x2b_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x2b,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.hl == 0xfffe);

    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.hl == 0xffff);
}

Test(inc_dec_instructions, _0x3b_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x3b,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xffff;

    step(&cpu);
    cr_assert(cpu.regs.sp == 0xfffe);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0x0;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xffff);
}


Test(inc_dec_instructions, _0x04_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x04,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.b = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.b == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.b = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.b == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}

Test(inc_dec_instructions, _0x0c_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x0c,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.c = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.c == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.c = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.c == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}

Test(inc_dec_instructions, _0x14_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x14,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.d = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.d == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.d = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.d == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}


Test(inc_dec_instructions, _0x1c_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x1c,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.e = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.e == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.e = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.e == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}

Test(inc_dec_instructions, _0x24_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x24,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.h = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.h == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.h = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.h == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}


Test(inc_dec_instructions, _0x2c_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x2c,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.l = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.l == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.l = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.l == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}

Test(inc_dec_instructions, _0x3c_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x3c,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.a = 0xff;

    step(&cpu);
    cr_assert(cpu.regs.a == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    cpu.regs.a = 0x10;
    step(&cpu);
    cr_assert(cpu.regs.a == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}

Test(inc_dec_instructions, _0x34_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x34,
        [0xabcd] = 0xff,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0xabcd;

    step(&cpu);
    cr_assert(ram[cpu.regs.hl] == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    ram[0xabcd] = 0x10;
    step(&cpu);
    cr_assert(ram[cpu.regs.hl] == 0x11);
    cr_assert(cpu.regs.f_z == 0);
}

Test(inc_dec_instructions, _0x35_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x35,
        [0xabcd] = 0xff,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.hl = 0xabcd;

    step(&cpu);
    cr_assert(ram[cpu.regs.hl] == 0xfe);
    cr_assert(cpu.regs.f_z == 0);

    cpu.regs.pc = 0x1234;
    ram[0xabcd] = 0x01;
    step(&cpu);
    cr_assert(ram[cpu.regs.hl] == 0x0);
    cr_assert(cpu.regs.f_z == 1);

    cpu.regs.pc = 0x1234;
    ram[0xabcd] = 0x00;
    step(&cpu);
    cr_assert(ram[cpu.regs.hl] == 0xff);
    cr_assert(cpu.regs.f_z == 0);
}
