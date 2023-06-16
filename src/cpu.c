#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mmu.h"
#include "cpu.h"
#include "instruction_functions.h"
#include "debugger.h"

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
        fprintf(stderr, "Unsupported read to IO\n");
    } else if (addr == 0xFFFF) {
        // interruption
        return cpu->ei_register;
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
        fprintf(stderr, "Unsupported write to IO\n");
    } else if (addr == 0xFFFF) {
        // interruption
        cpu->ei_register = value;
        fprintf(stderr, "Unsupported write to interruption\n");
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
        cpu->mmu.rom_buffer[addr] = (value >> 8) & 0xff;
        cpu->mmu.rom_buffer[addr+1] = value & 0xff;
    }
    fprintf(stderr, "Unsupported write16 \n");
    return;
}

Instruction instructions[0x100] = {
    [0x00] = {INSTRUCTION_NOP, ADDRESSING_MODE_NONE},
    [0x01] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_BC},
    [0x02] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R, REGISTER_NAME_BC, REGISTER_NAME_A},
    [0x03] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_BC},
    [0x04] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_B},
    [0x05] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_B},

    [0x06] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_B},

    [0x08] = {INSTRUCTION_LD, ADDRESSING_MODE_A16_R, REGISTER_NAME_NONE, REGISTER_NAME_SP},
    [0x0C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_C},

    [0x0A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_BC},
    [0x0B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_BC},
    [0x0D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_C},
    [0x0E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_C},

    [0x11] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_DE},
    [0x12] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R, REGISTER_NAME_DE, REGISTER_NAME_A},
    [0x13] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_DE},
    [0x14] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0x15] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0x16] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_D},
    [0x18] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8},
    [0x1A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_DE},
    [0x1B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_DE},
    [0x1C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0x1D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0x1E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_E},

    [0x20] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0x21] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_HL},
    [0x22] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R,  REGISTER_NAME_HL, REGISTER_NAME_A},
    [0x23] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_HL},
    [0x24] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_H},
    [0x25] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_H},
    [0x26] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_H},
    [0x28] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0x2A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR,  REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x2B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_HL},
    [0x2C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_L},
    [0x2D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_L},
    [0x2E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_L},

    [0x30] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0x31] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_SP},
    [0x32] = {INSTRUCTION_LD, ADDRESSING_MODE_HLD_R,  REGISTER_NAME_HL, REGISTER_NAME_A},
    [0x33] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_SP},
    [0x34] = {INSTRUCTION_INC, ADDRESSING_MODE_MR, REGISTER_NAME_HL},
    [0x35] = {INSTRUCTION_DEC, ADDRESSING_MODE_MR,    REGISTER_NAME_HL},
    [0x36] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_H},
    [0x38] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0x3A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_HLD,  REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x3B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_SP},
    [0x3C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_A},
    [0x3D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_A},
    [0x3E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_A},

    [0x40] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_B},
    [0x41] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_C},
    [0x42] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_D},
    [0x43] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_E},
    [0x44] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_H},
    [0x45] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_L},
    [0x46] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_B, REGISTER_NAME_HL},
    [0x47] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_B, REGISTER_NAME_A},
    [0x48] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_B},
    [0x49] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_C},
    [0x4A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_D},
    [0x4B] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_E},
    [0x4C] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_H},
    [0x4D] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_L},
    [0x4E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_C, REGISTER_NAME_HL},
    [0x4F] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_C, REGISTER_NAME_A},

    [0x50] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_B},
    [0x51] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_C},
    [0x52] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_D},
    [0x53] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_E},
    [0x54] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_H},
    [0x55] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_L},
    [0x56] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_D, REGISTER_NAME_HL},
    [0x57] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_D, REGISTER_NAME_A},
    [0x58] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_B},
    [0x59] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_C},
    [0x5A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_D},
    [0x5B] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_E},
    [0x5C] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_H},
    [0x5D] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_L},
    [0x5E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_E, REGISTER_NAME_HL},
    [0x5F] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_E, REGISTER_NAME_A},

    [0x60] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_B},
    [0x61] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_C},
    [0x62] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_D},
    [0x63] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_E},
    [0x64] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_H},
    [0x65] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_L},
    [0x66] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_H, REGISTER_NAME_HL},
    [0x67] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_H, REGISTER_NAME_A},
    [0x68] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_B},
    [0x69] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_C},
    [0x6A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_D},
    [0x6B] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_E},
    [0x6C] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_H},
    [0x6D] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_L},
    [0x6E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_L, REGISTER_NAME_HL},
    [0x6F] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_L, REGISTER_NAME_A},

    [0x70] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_B},
    [0x71] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_C},
    [0x72] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_D},
    [0x73] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_E},
    [0x74] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_H},
    [0x75] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_L},
    [0x76] = {INSTRUCTION_HALT},
    [0x77] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_A},
    [0x78] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_B},
    [0x79] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_C},
    [0x7A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_D},
    [0x7B] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_E},
    [0x7C] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_H},
    [0x7D] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_L},
    [0x7E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x7F] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_A},

    [0xAF] = {INSTRUCTION_XOR, ADDRESSING_MODE_R, REGISTER_NAME_A},

    [0xC0] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0xC1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_BC},
    [0xC2] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0xC3] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16},
    [0xC4] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0xC5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_BC},
    [0xC7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x0},
    [0xC8] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0xC9] = {INSTRUCTION_RET},
    [0xCA] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0xCC] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0xCD] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16},
    [0xCF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x0f},

    [0xD0] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0xD1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_DE},
    [0xD2] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0xD4] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0xD5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_DE},
    [0xD7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x10},
    [0xD8] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0xD9] = {INSTRUCTION_RETI},
    [0xDA] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0xDC] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0xDF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x18},

    [0xE0] = {INSTRUCTION_LDH, ADDRESSING_MODE_A8_R, REGISTER_NAME_NONE, REGISTER_NAME_A},
    [0xE1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_HL},
    [0xE2] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R, REGISTER_NAME_C, REGISTER_NAME_A},
    [0xE5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_HL},
    [0xE7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x20},
    [0xE9] = {INSTRUCTION_JP, ADDRESSING_MODE_R, REGISTER_NAME_HL},
    [0xEA] = {INSTRUCTION_LD, ADDRESSING_MODE_A16_R, REGISTER_NAME_NONE, REGISTER_NAME_A},
    [0xEF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x28},

    [0xF0] = {INSTRUCTION_LDH, ADDRESSING_MODE_R_A8, REGISTER_NAME_A, REGISTER_NAME_NONE},
    [0xF1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_AF},
    [0xF2] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_C},
    [0xF3] = {INSTRUCTION_DI},
    [0xF5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_AF},
    [0xF7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x30},
    [0xFA] = {INSTRUCTION_LD, ADDRESSING_MODE_R_A16, REGISTER_NAME_A},
    [0xFF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x38},
};

cpu_instruction_function_ptr* instruction_ptrs[] = {
    [INSTRUCTION_NONE] = invalid_instruction,
    [INSTRUCTION_NOP] = nop_instruction,
    [INSTRUCTION_LD] = ld_instruction,
    [INSTRUCTION_LDH] = ldh_instruction,
    [INSTRUCTION_JP] = jp_instruction,
    [INSTRUCTION_JR] = jr_instruction,
    [INSTRUCTION_DI] = di_instruction,
    [INSTRUCTION_XOR] = xor_instruction,
    [INSTRUCTION_POP] = pop_instruction,
    [INSTRUCTION_PUSH] = push_instruction,
    [INSTRUCTION_CALL] = call_instruction,
    [INSTRUCTION_RET] = ret_instruction,
    [INSTRUCTION_RETI] = reti_instruction,
    [INSTRUCTION_RST] = rst_instruction,
    [INSTRUCTION_INC] = inc_instruction,
    [INSTRUCTION_DEC] = dec_instruction,
    [INSTRUCTION_ADD] = add_instruction
};

void fetch_opcode(Cpu* cpu) {
    cpu->current_opcode = mmu_read(cpu, cpu->regs.pc);
    cpu->regs.pc++;
}

uint16_t get_reg(Cpu* cpu, RegisterName reg_name) {
    switch(reg_name) {
        case REGISTER_NAME_A: return cpu->regs.a;
        case REGISTER_NAME_B: return cpu->regs.b;
        case REGISTER_NAME_C: return cpu->regs.c;
        case REGISTER_NAME_D: return cpu->regs.d;
        case REGISTER_NAME_E: return cpu->regs.e;
        case REGISTER_NAME_F: return cpu->regs.f;
        case REGISTER_NAME_H: return cpu->regs.h;
        case REGISTER_NAME_L: return cpu->regs.l;
        case REGISTER_NAME_AF: return cpu->regs.af;
        case REGISTER_NAME_BC: return cpu->regs.bc;
        case REGISTER_NAME_DE: return cpu->regs.de;
        case REGISTER_NAME_HL: return cpu->regs.hl;
        case REGISTER_NAME_PC: return cpu->regs.pc;
        case REGISTER_NAME_SP: return cpu->regs.sp;
        default:
            fprintf(stderr, "ERROR UNKNOWN REGISTER NAME\n");
            exit(1);
    }
    fprintf(stderr, "ERROR UNKNOWN REGISTER NAME\n");
    exit(1);
}

void set_reg(Cpu* cpu, RegisterName reg_name, uint16_t value) {
    switch(reg_name) {
        case REGISTER_NAME_A: cpu->regs.a = value & 0xff; break;
        case REGISTER_NAME_B: cpu->regs.b = value & 0xff; break;
        case REGISTER_NAME_C: cpu->regs.c = value & 0xff; break;
        case REGISTER_NAME_D: cpu->regs.d = value & 0xff; break;
        case REGISTER_NAME_E: cpu->regs.e = value & 0xff; break;
        case REGISTER_NAME_F: cpu->regs.f = value & 0xff; break;
        case REGISTER_NAME_H: cpu->regs.h = value & 0xff; break;
        case REGISTER_NAME_L: cpu->regs.l = value & 0xff; break;
        case REGISTER_NAME_AF: cpu->regs.af = value; break;
        case REGISTER_NAME_BC: cpu->regs.bc = value; break;
        case REGISTER_NAME_DE: cpu->regs.de = value; break;
        case REGISTER_NAME_HL: cpu->regs.hl = value; break;
        case REGISTER_NAME_PC: cpu->regs.pc = value; break;
        case REGISTER_NAME_SP: cpu->regs.sp = value; break;
        default:
            fprintf(stderr, "ERROR UNKNOWN REGISTER NAME\n");
            exit(1);
    }
}

void fetch_data(Cpu* cpu) {
    switch (cpu->current_instruction->mode) {
        case ADDRESSING_MODE_NONE:
            cpu->current_data = 0;
            return;
        case ADDRESSING_MODE_R_D8:
        case ADDRESSING_MODE_IMM8:
            cpu->current_data = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles += 1;
            cpu->regs.pc++;
            return;
        case ADDRESSING_MODE_IMM16:
        case ADDRESSING_MODE_A16:
        case ADDRESSING_MODE_R_D16: {
            uint8_t lo = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles += 1;
            uint8_t hi = mmu_read(cpu, cpu->regs.pc + 1);
            cpu->cycles += 1;
            cpu->current_data = lo | (hi << 8);
            cpu->regs.pc += 2;
            return;
        }
        case ADDRESSING_MODE_R:
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_a);
            return;
        case ADDRESSING_MODE_R_R:
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_b);
            return;
        case ADDRESSING_MODE_MR_R:
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_b);
            cpu->current_memory_destination = get_reg(cpu, cpu->current_instruction->reg_a);
            cpu->current_destination_in_memory = true;
            if (cpu->current_instruction->reg_a == REGISTER_NAME_C) {
                cpu->current_memory_destination |= 0xff00;
            }
            return;
        case ADDRESSING_MODE_R_MR: {
            uint16_t address = get_reg(cpu, cpu->current_instruction->reg_b);
            if (cpu->current_instruction->reg_b == REGISTER_NAME_C) {
                address |= 0xff00;
            }
            cpu->current_data = mmu_read(cpu, address);
            cpu->cycles++;
            return;
        }
        case ADDRESSING_MODE_R_HLI:
            cpu->current_data = mmu_read(cpu, get_reg(cpu, cpu->current_instruction->reg_b));
            cpu->cycles++;
            cpu->regs.hl++;
            return;
        case ADDRESSING_MODE_R_HLD:
            cpu->current_data = mmu_read(cpu, get_reg(cpu, cpu->current_instruction->reg_b));
            cpu->cycles++;
            cpu->regs.hl--;
            return;
        case ADDRESSING_MODE_HLI_R:
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_b);
            cpu->cycles++;
            cpu->current_memory_destination = get_reg(cpu, cpu->current_instruction->reg_a);
            cpu->current_destination_in_memory = true;
            cpu->regs.hl++;
            return;
        case ADDRESSING_MODE_HLD_R:
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_b);
            cpu->cycles++;
            cpu->current_memory_destination = get_reg(cpu, cpu->current_instruction->reg_a);
            cpu->current_destination_in_memory = true;
            cpu->regs.hl--;
            return;
        case ADDRESSING_MODE_R_A8:
            cpu->current_data = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles++;
            cpu->regs.pc++;
            return;
        case ADDRESSING_MODE_A8_R:
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_a);
            cpu->current_memory_destination = 0xff00 | mmu_read(cpu, cpu->regs.pc);
            cpu->cycles++;
            cpu->current_destination_in_memory = true;
            cpu->regs.pc++;
            return;
        case ADDRESSING_MODE_D8:
        case ADDRESSING_MODE_HL_SPR:
            cpu->current_data = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles++;
            cpu->regs.pc++;
            return;
        case ADDRESSING_MODE_A16_R:
        case ADDRESSING_MODE_D16_R: {
            uint8_t lo = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles += 1;
            uint8_t hi = mmu_read(cpu, cpu->regs.pc + 1);
            cpu->cycles += 1;
            cpu->current_memory_destination = lo | (hi << 8);
            cpu->current_destination_in_memory = true;
            cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_b);
            cpu->regs.pc += 2;
            return;
        }
        case ADDRESSING_MODE_MR_D8: {
            cpu->current_data = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles += 1;
            cpu->regs.pc++;
            cpu->current_memory_destination = get_reg(cpu, cpu->current_instruction->reg_a);
            cpu->current_destination_in_memory = true;
            return;
        }
        case ADDRESSING_MODE_MR: {
            cpu->current_memory_destination = get_reg(cpu, cpu->current_instruction->reg_a);
            cpu->current_destination_in_memory = true;
            cpu->current_data = mmu_read(cpu, get_reg(cpu, cpu->current_instruction->reg_a));
            cpu->cycles += 1;
            return;
        }
        case ADDRESSING_MODE_R_A16: {
            uint8_t lo = mmu_read(cpu, cpu->regs.pc);
            cpu->cycles += 1;
            uint8_t hi = mmu_read(cpu, cpu->regs.pc + 1);
            cpu->cycles += 1;
            uint16_t address = lo | (hi << 8);
            cpu->current_data = mmu_read(cpu, address);
            cpu->cycles++;
            cpu->regs.pc += 2;
            return;
        }

        default:
            fprintf(stderr, "UNKNOWN ADDRESSING MODE %d\n", cpu->current_instruction->mode);
            exit(1);
    }
}

void execute(Cpu* cpu) {
    cpu_instruction_function_ptr* instruction_ptr = instruction_ptrs[cpu->current_instruction->type];
    if (instruction_ptr == INSTRUCTION_NONE) {
        fprintf(stderr, "UNKNOWN INSTRUCTION WHILE EXECUTING %d\n", cpu->current_instruction->type);
        exit(1);
    }
    instruction_ptr(cpu);
}

void get_current_instruction(Cpu* cpu) {
    Instruction* ins = &(instructions[cpu->current_opcode]);
    if (ins->type == INSTRUCTION_NONE) {
        fprintf(stderr, "UNKNOWN INSTRUCTION WHILE GETTING INSTRUCTION 0x%02x\n", cpu->current_opcode);
        exit(1);
    }
    cpu->current_instruction = ins;
}

void step(Cpu* cpu) {
    printf("0x%04x : ", cpu->regs.pc);
    fetch_opcode(cpu);
    get_current_instruction(cpu);
    printf("%s\t",
        get_instruction_type_name(cpu->current_instruction->type)
    );
    printf("(%02X %02X %02X) ",
            cpu->current_opcode,
            mmu_read(cpu, cpu->regs.pc),
            mmu_read(cpu, cpu->regs.pc+1));
    fetch_data(cpu);
    execute(cpu);
    debug_registers_inline(&cpu->regs);
    putchar('\n');
}

void stack_push(Cpu* cpu, uint8_t value) {
    cpu->regs.sp--;
    mmu_write(cpu, cpu->regs.sp, value);
}

void stack_push16(Cpu* cpu, uint16_t value) {
    stack_push(cpu, (value >> 8) & 0xff);
    stack_push(cpu, value & 0xff);
}

uint8_t stack_pop(Cpu* cpu) {
    uint8_t value = mmu_read(cpu, cpu->regs.sp);
    cpu->regs.sp++;
    return value;
}

uint16_t stack_pop16(Cpu* cpu) {
    uint16_t hi = stack_pop(cpu);
    uint16_t lo = stack_pop(cpu);
    return (hi << 8) | lo;
}
