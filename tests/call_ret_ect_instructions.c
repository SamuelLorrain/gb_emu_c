#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"

Test(call_ret_etc_instructions, _0xcd_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xcd,
        [0x1235] = 0x9a,
        [0x1236] = 0x78,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x789a);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x37);
}

Test(call_ret_etc_instructions, _0xc9_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xc9,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0xff90;
    cpu.mmu.hram_buffer[0xff90 - 0xff80] = 0xcd;
    cpu.mmu.hram_buffer[0xff91 - 0xff80] = 0xab;
    cpu.regs.pc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff92);
    cr_assert(cpu.regs.pc == 0xabcd);
}

Test(call_ret_etc_instructions, _call_then_ret_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x0] = 0xcd,    // CALL 0x1234
        [0x1] = 0x34,
        [0x2] = 0x12,
        [0x1234] = 0x06, // LOAD B, d8
        [0x1235] = 0xaa,
        [0x1236] = 0xc9, // RET
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x0;
    cpu.regs.sp = 0xffff;
    step(&cpu);
    step(&cpu);
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x3);
    cr_assert(cpu.regs.b == 0xaa);
    cr_assert(cpu.regs.sp == 0xffff);
    cr_assert(cpu.mmu.hram_buffer[0xfffd - 0xff80] == 0x03);
    cr_assert(cpu.mmu.hram_buffer[0xfffe - 0xff80] == 0x00);
}

Test(call_ret_etc_instructions, _0xc4_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xc4,
        [0x1235] = 0x9a,
        [0x1236] = 0x78,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x789a);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x37);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x1237);
    cr_assert(cpu.regs.sp == 0xfffd);
}

Test(call_ret_etc_instructions, _0xcc_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xcc,
        [0x1235] = 0x9a,
        [0x1236] = 0x78,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x789a);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x37);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x1237);
    cr_assert(cpu.regs.sp == 0xfffd);
}

Test(call_ret_etc_instructions, _0xd4_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xd4,
        [0x1235] = 0x9a,
        [0x1236] = 0x78,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x789a);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x37);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x1237);
    cr_assert(cpu.regs.sp == 0xfffd);
}

Test(call_ret_etc_instructions, _0xdc_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xdc,
        [0x1235] = 0x9a,
        [0x1236] = 0x78,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x789a);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x37);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x1237);
    cr_assert(cpu.regs.sp == 0xfffd);
}

Test(call_ret_etc_instructions, _0xc0_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xc0,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_z = 0;
    cpu.mmu.hram_buffer[0xff90 - 0xff80] = 0xcd;
    cpu.mmu.hram_buffer[0xff91 - 0xff80] = 0xab;
    cpu.regs.pc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff92);
    cr_assert(cpu.regs.pc == 0xabcd);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_z = 1;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff90);
    cr_assert(cpu.regs.pc == 0x1235);
}

Test(call_ret_etc_instructions, _0xc8_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xc8,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_z = 1;
    cpu.mmu.hram_buffer[0xff90 - 0xff80] = 0xcd;
    cpu.mmu.hram_buffer[0xff91 - 0xff80] = 0xab;
    cpu.regs.pc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff92);
    cr_assert(cpu.regs.pc == 0xabcd);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_z = 0;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff90);
    cr_assert(cpu.regs.pc == 0x1235);
}

Test(call_ret_etc_instructions, _0xd0_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xd0,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_c = 0;
    cpu.mmu.hram_buffer[0xff90 - 0xff80] = 0xcd;
    cpu.mmu.hram_buffer[0xff91 - 0xff80] = 0xab;
    cpu.regs.pc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff92);
    cr_assert(cpu.regs.pc == 0xabcd);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_c = 1;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff90);
    cr_assert(cpu.regs.pc == 0x1235);
}

Test(call_ret_etc_instructions, _0xd8_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xd8,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_c = 1;
    cpu.mmu.hram_buffer[0xff90 - 0xff80] = 0xcd;
    cpu.mmu.hram_buffer[0xff91 - 0xff80] = 0xab;
    cpu.regs.pc = 0x1234;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff92);
    cr_assert(cpu.regs.pc == 0xabcd);

    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xff90;
    cpu.regs.f_c = 0;
    step(&cpu);
    cr_assert(cpu.regs.sp == 0xff90);
    cr_assert(cpu.regs.pc == 0x1235);
}

Test(call_ret_etc_instructions, _0xc7_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xc7,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0000);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xcf_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xcf,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0008);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xd7_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xd7,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0010);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xdf_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xdf,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0018);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xe7_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xe7,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0020);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xef_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xef,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0028);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xf7_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xf7,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0030);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}

Test(call_ret_etc_instructions, _0xff_instruction) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0xff,
    };
    cpu.mmu.rom_buffer = ram;
    cpu.regs.pc = 0x1234;
    cpu.regs.sp = 0xfffd;
    step(&cpu);
    cr_assert(cpu.regs.pc == 0x0038);
    cr_assert(cpu.regs.sp == 0xfffb);
    cr_assert(cpu.mmu.hram_buffer[0xfffc - 0xff80] == 0x12);
    cr_assert(cpu.mmu.hram_buffer[0xfffb - 0xff80] == 0x35);
}
