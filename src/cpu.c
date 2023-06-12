#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mmu.h"
#include "cpu.h"
#include "instruction.h"
#include "instruction_functions.h"

Instruction instructions[0x100] = {
    [0x00] = {INSTRUCTION_NOP, ADDRESSING_MODE_NONE},
    [0x30] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0x31] = {INSTRUCTION_LD, ADDRESSING_MODE_R_IMM16, REGISTER_NAME_SP},
    [0xc3] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16},
    [0xf3] = {INSTRUCTION_DI, ADDRESSING_MODE_NONE}
};

cpu_instruction_function_ptr* instruction_ptrs[] = {
    [INSTRUCTION_NONE] = invalid_instruction,
    [INSTRUCTION_NOP] = nop_instruction,
    [INSTRUCTION_LD] = ld_instruction,
    [INSTRUCTION_JP] = jp_instruction,
    [INSTRUCTION_JR] = jr_instruction,
    [INSTRUCTION_DI] = di_instruction,
};

void fetch_opcode(Cpu* cpu) {
    cpu->current_opcode = mmu_read(&cpu->mmu, cpu->regs.pc);
    cpu->regs.pc++;
}

void fetch_data(Cpu* cpu) {
    switch (cpu->current_instruction->mode) {
        case ADDRESSING_MODE_NONE:
            cpu->current_data = 0;
            return;
        case ADDRESSING_MODE_IMM16: {
            uint8_t lo = mmu_read(&cpu->mmu, cpu->regs.pc);
            cpu->cycles += 1;
            uint8_t hi = mmu_read(&cpu->mmu, cpu->regs.pc + 1);
            cpu->cycles += 1;
            cpu->regs.pc += 2;
            cpu->current_data = (hi << 8) | lo;
            return;
        }
        case ADDRESSING_MODE_IMM8:
            cpu->current_data = mmu_read(&cpu->mmu, cpu->regs.pc);
            cpu->cycles += 1;
            cpu->regs.pc++;
            return;
        case ADDRESSING_MODE_R:
        case ADDRESSING_MODE_R_IMM8:
        case ADDRESSING_MODE_R_R:
            cpu->current_data = 0;
            return;
        default:
            fprintf(stderr, "UNKNOWN ADDRESSING MODE\n");
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
        fprintf(stderr, "UNKNOWN INSTRUCTION WHILE GETTING INSTRUCTION %x\n", cpu->current_opcode);
        exit(1);
    }
    cpu->current_instruction = ins;
}

void step(Cpu* cpu) {
    printf("============\n");
    printf("current pc 0x%04x, %u\n", cpu->regs.pc);
    fetch_opcode(cpu);
    printf("current opcode: 0x%02x\n", cpu->current_opcode);
    get_current_instruction(cpu);
    printf("current instruction: %x\n", cpu->current_instruction->type);
    fetch_data(cpu);
    printf("current data: %x\n", cpu->current_data);
    execute(cpu);
    printf("============\n");
}

