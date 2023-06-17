#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "instruction_functions.h"
#include "debugger.h"
#include "mmu.h"

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
            fprintf(stderr, "ERROR UNKNOWN REGISTER NAME (get_reg)\n");
            exit(1);
    }
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
            fprintf(stderr, "ERROR UNKNOWN REGISTER NAME (set_reg)\n");
            exit(1);
    }
}

void fetch_data(Cpu* cpu) {
    switch (cpu->current_instruction->mode) {
        case ADDRESSING_MODE_NONE:
            cpu->current_data = 0;
            return;
        case ADDRESSING_MODE_R_D8:
        case ADDRESSING_MODE_A8:
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
            /* cpu->current_data = get_reg(cpu, cpu->current_instruction->reg_a); */
            cpu->current_data = mmu_read(cpu, cpu->regs.pc);
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
    extern cpu_instruction_function_ptr* instruction_ptrs[];
    cpu_instruction_function_ptr* instruction_ptr = instruction_ptrs[cpu->current_instruction->type];
    if (instruction_ptr == INSTRUCTION_NONE) {
        fprintf(stderr, "UNKNOWN INSTRUCTION WHILE EXECUTING 0x%02x\n", cpu->current_instruction->type);
        exit(1);
    }
    instruction_ptr(cpu);
}

void get_current_instruction(Cpu* cpu) {
    extern Instruction instructions[0x100];
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

void reset_instruction_state(Cpu* cpu) {
    cpu->current_data = 0;
    cpu->current_opcode = 0;
    cpu->current_memory_destination = 0;
    cpu->current_destination_in_memory = 0;
    cpu->current_instruction = (void*)0;
}

