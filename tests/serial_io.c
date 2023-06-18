#include <criterion/criterion.h>
#include <stdio.h>
#include "cpu.h"

Test(serial_io, _read_char_from_serial_io) {
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
