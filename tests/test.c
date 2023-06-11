#include <criterion/criterion.h>
#include "cpu.h"

Test(cpu_instructions, instruction_0x01_works_as_expected) {
    Registers regs = {0};
    unsigned char ram[0xffff] = {
        0x01,
        0xAB,
        0xCD
    };
    execute_next(&regs, ram);
    cr_assert(regs.bc == 0xCDAB);
    cr_assert(regs.pc == 3);
}

Test(cpu_instructions, instruction_0x02_works_as_expected) {
    Registers regs = { .bc = 0x200, .pc = 0xAAA, .a = 0xBF };
    unsigned char ram[0xffff] = {
        [0x200] = 0xAA,
        [0xAAA] = 0x02, // opcode
    };
    execute_next(&regs, ram);
    cr_assert(ram[regs.bc] == 0xBF);
    cr_assert(regs.pc == 0xAAB);
}

Test(cpu_instructions, instruction_0x03_works_as_expected) {
    Registers regs = { .bc = 0x200, .pc = 0xAAA };
    unsigned char ram[0xffff] = {
        [0xAAA] = 0x03, // opcode
    };
    execute_next(&regs, ram);
    cr_assert(regs.bc == 0x201);
    cr_assert(regs.pc == 0xAAB);

    Registers regs2 = { .bc = 0xFFFF, .pc = 0xAAA };
    execute_next(&regs2, ram);
    cr_assert(regs2.bc == 0x0);
    cr_assert(regs2.pc == 0xAAB);
}

Test(cpu_instructions, instruction_0x04_works_as_expected) {
    Registers regs = { .b = 0xff, .pc = 0xaaa };
    unsigned char ram[0xffff] = {
        [0xaaa] = 0x04, // opcode
    };
    execute_next(&regs, ram);
    cr_assert(regs.b == 0x0);
    cr_assert(regs.f_z == 0x1);
    cr_assert(regs.f_n == 0x0);
    cr_assert(regs.f_h == 0x1);
    cr_assert(regs.pc == 0xaab);

    regs.b = 0x0a;
    regs.pc = 0xaaa;
    execute_next(&regs, ram);
    cr_assert(regs.b == 0x0b);
    cr_assert(regs.f_z == 0x0);
    cr_assert(regs.f_n == 0x0);
    cr_assert(regs.f_h == 0x0);
    cr_assert(regs.pc == 0xaab);
}

Test(cpu_instructions, instruction_0x05_works_as_expected) {
    Registers regs = { .b = 0xff, .pc = 0xaaa };
    unsigned char ram[0xffff] = {
        [0xaaa] = 0x05, // opcode
    };
    execute_next(&regs, ram);
    cr_assert(regs.b == 0xfe);
    cr_assert(regs.f_z == 0x0);
    cr_assert(regs.f_n == 0x1);
    cr_assert(regs.f_h == 0x0);
    cr_assert(regs.pc == 0xaab);

    regs.b = 0x10;
    regs.pc = 0xaaa;
    execute_next(&regs, ram);
    cr_assert(regs.b == 0x0f);
    cr_assert(regs.f_z == 0x0);
    cr_assert(regs.f_n == 0x1);
    cr_assert(regs.f_h == 0x0);
    cr_assert(regs.pc == 0xaab);

    regs.b = 0x01;
    regs.pc = 0xaaa;
    execute_next(&regs, ram);
    cr_assert(regs.b == 0x00);
    cr_assert(regs.f_z == 0x1);
    cr_assert(regs.f_n == 0x1);
    cr_assert(regs.f_h == 0x1);
    cr_assert(regs.pc == 0xaab);

    regs.b = 0x11;
    regs.pc = 0xaaa;
    execute_next(&regs, ram);
    cr_assert(regs.b == 0x10);
    cr_assert(regs.f_z == 0x0);
    cr_assert(regs.f_n == 0x1);
    cr_assert(regs.f_h == 0x1);
    cr_assert(regs.pc == 0xaab);
}

Test(cpu_instructions, instruction_0x06_works_as_expected) {
    Registers regs = { .b = 0xff, .pc = 0xaaa };
    unsigned char ram[0xffff] = {
        [0xaaa] = 0x06, // opcode
        [0xaab] = 0x8C
    };
    execute_next(&regs, ram);
    cr_assert(regs.b == 0x8c);
    cr_assert(regs.pc == 0xaac);
}

Test(cpu_instructions, instruction_0x07_works_as_expected) {
    Registers regs = { .a = 0xff, .pc = 0xaaa };
    unsigned char ram[0xffff] = {
        [0xaaa] = 0x07, // opcode
    };
    execute_next(&regs, ram);
    cr_assert(regs.a == 0xff);
    cr_assert(regs.pc == 0xaab);
    cr_assert(regs.f_z == 0);
    cr_assert(regs.f_h == 0);
    cr_assert(regs.f_n == 0);
    cr_assert(regs.f_c == 1);

    regs.a = 0x01;
    regs.pc = 0xaaa;
    execute_next(&regs, ram);
    cr_assert(regs.a == 0x02);
    cr_assert(regs.pc == 0xaab);
    cr_assert(regs.f_c == 0);

    regs.a = 0x80;
    regs.pc = 0xaaa;
    execute_next(&regs, ram);
    cr_assert(regs.a == 0x1);
    cr_assert(regs.f_c == 1);
    cr_assert(regs.pc == 0xaab);
}

Test(cpu_instructions, instruction_0x08_works_as_expected) {
    Registers regs = { .sp = 0x1234, .pc = 0x200 };
    unsigned char ram[0xffff] = {
        [0x200] = 0x08, // opcode
        [0x201] = 0xCD,
        [0x202] = 0xAB,
    };
    execute_next(&regs, ram);

    cr_assert(regs.pc == 0x203);
    cr_assert(ram[0xABCD] == 0x34);
    cr_assert(ram[0xABCE] == 0x12);
}
