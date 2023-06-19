#include <criterion/criterion.h>
#include <stdio.h>
#include "mmu.h"
#include "cpu.h"

Test(serial_io, _read_char_from_serial_io) {
    Cpu cpu = {0};
    uint8_t ram[0xffff] = {
        [0x1234] = 0x00,
    };
    cpu.mmu.rom_buffer = ram;
    mmu_write(&cpu, 0xFF01, 'k');
    mmu_write(&cpu, 0xFF02, 0x81);
    step(&cpu);
    cr_assert(get_debug_serial_buffer(0) == 'k');
}
