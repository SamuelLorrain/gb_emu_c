#include <stdio.h>
#include "mmu.h"
#include "cpu.h"

uint8_t mmu_read(Cpu* cpu, uint16_t addr) {
    if (addr < 0x8000) {
        return cpu->mmu.rom_buffer[addr];
    } else if (addr <  0xA000) {
        fprintf(stderr, "Unsupported read to Char/map data\n");
        return -1;
    } else if (addr <  0xC000) {
        // cartridge ram
        return cpu->mmu.rom_buffer[addr];
    } else if (addr <  0xE000) {
        // wram
        return cpu->mmu.wram_buffer[addr - 0xC0000];
    } else if (addr <  0xFE00) {
        // echo ram
        return 0;
    } else if (addr <  0xFEA0) {
        // oam
        fprintf(stderr, "Unsupported read to OAM\n");
        return 0;
    } else if (addr <  0xFF00) {
        // reserved
        return 0;
    } else if (addr <  0xFF80) {
        // I/O registers
        if (addr == 0xff01) {
            return cpu->mmu.serial_data[0];
        } else if (addr == 0xff02) {
            return cpu->mmu.serial_data[1];
        }
        fprintf(stderr, "Unsupported read to io\n");
    } else if (addr == 0xFFFF) {
        // interruption
        return cpu->mmu.ei_register;
        fprintf(stderr, "Unsupported read to interruption\n");
    } else {
        // >= FF80 && < FFFF
        return cpu->mmu.hram_buffer[addr - 0xFF80];
    }
    return -1;
}

void mmu_write(Cpu* cpu, uint16_t addr, uint8_t value) {
    if (addr < 0x8000) {
        cpu->mmu.rom_buffer[addr] = value;
    } else if (addr <  0xA000) {
        fprintf(stderr, "Unsupported write to Char/map data\n");
        return;
    } else if (addr <  0xC000) {
        // cartridge ram
        cpu->mmu.rom_buffer[addr] = value;
    } else if (addr <  0xE000) {
        // wram
        printf("wram address write 0x%04x ==> 0x%04x\n", addr, addr - 0xC000);
        cpu->mmu.wram_buffer[addr - 0xC0000] = value;
    } else if (addr <  0xFE00) {
        // echo ram
        return;
    } else if (addr <  0xFEA0) {
        // oam
        fprintf(stderr, "Unsupported write to OAM\n");
        return;
    } else if (addr <  0xFF00) {
        // reserved
        return;
    } else if (addr <  0xFF80) {
        // I/O registers
        if (addr == 0xff01) {
            cpu->mmu.serial_data[0] = value;
            return;
        } else if (addr == 0xff02) {
            cpu->mmu.serial_data[1] = value;
            return;
        }
        fprintf(stderr, "Unsupported write to IO\n");
    } else if (addr == 0xFFFF) {
        cpu->mmu.ei_register = value;
    }
    // >= FF80 && < FFFF
    cpu->mmu.hram_buffer[addr - 0xFF80] = value;
}

uint16_t mmu_read16(Cpu* cpu, uint16_t addr) {
    // little endian
    if (addr < 0x8000) {
        return (cpu->mmu.rom_buffer[addr]) | (cpu->mmu.rom_buffer[addr+1] << 8);
    }
    fprintf(stderr, "Unsupported read\n");
    return -1;
}

void mmu_write16(Cpu* cpu, uint16_t addr, uint16_t value) {
    // little endian
    if (addr+1 < 0x8000) {
        cpu->mmu.rom_buffer[addr] = value & 0xff;
        cpu->mmu.rom_buffer[addr+1] = (value >> 8) & 0xff;
    } else {
        fprintf(stderr, "Unsupported write16 \n");
        return;
    }
}

