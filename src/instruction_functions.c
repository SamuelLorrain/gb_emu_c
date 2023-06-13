#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "instruction_functions.h"

bool check_condition(Cpu* cpu) {
    ConditionFlag condition = cpu->current_instruction->condition;
    uint8_t f_z = cpu->regs.f_z;
    uint8_t f_c = cpu->regs.f_c;
    if (condition == CONDITION_FLAG_NONE) {
        return true;
    } else if (condition == CONDITION_FLAG_Z && f_z == 1) {
        return true;
    } else if (condition == CONDITION_FLAG_NZ && f_z == 0) {
        return true;
    } else if (condition == CONDITION_FLAG_C && f_c == 1) {
        return true;
    } else if (condition == CONDITION_FLAG_NC && f_c == 0) {
        return true;
    }
    return false;
}

void invalid_instruction(Cpu* cpu) {
    fprintf(stderr, "INVALID INSTRUCTION %x\n", cpu->current_opcode);
    exit(1);
}

void ld_instruction(Cpu* cpu) {
    if (cpu->current_destination_in_memory) {
        if (cpu->current_instruction->reg_b >= REGISTER_NAME_AF) {
            cpu->cycles++;
            mmu_write16(cpu, cpu->current_destination_in_memory, cpu->current_data);
        } else {
            mmu_write(cpu, cpu->current_destination_in_memory, cpu->current_data);
        }
        return;
    }
    if (cpu->current_instruction->mode == ADDRESSING_MODE_HL_SPR) {
        uint8_t b = get_reg(cpu, cpu->current_instruction->reg_b);
        uint8_t a = cpu->current_data;
        cpu->regs.f_h = (b & 0xf) + (a & 0xf) >= 0x10;
        cpu->regs.f_c = (b & 0xff) + (a & 0xff) >= 0x100;
        set_reg(cpu,
                cpu->current_instruction->reg_a,
                cpu->current_instruction->reg_b + cpu->current_data);
        return;
    }
    // general case
    set_reg(cpu, cpu->current_instruction->reg_a, cpu->current_data);
}

void ldh_instruction(Cpu* cpu) {
    if (cpu->current_instruction->reg_a == REGISTER_NAME_A) {
        set_reg(
            cpu,
            REGISTER_NAME_A,
            mmu_read(cpu,
            0xff00 | cpu->current_data)
        );
    } else {
        mmu_write(cpu, 0xff00 | cpu->current_data, cpu->regs.a);
    }
    cpu->cycles++;
}

void jp_instruction(Cpu* cpu) {
    if (check_condition(cpu)) {
        cpu->regs.pc = cpu->current_data;
        cpu->cycles += 1;
    }
}

void jr_instruction(Cpu* cpu) {
    if (check_condition(cpu)) {
        cpu->regs.pc += cpu->current_data;
        cpu->cycles += 1;
    }
}

void nop_instruction(void) {}

void di_instruction(Cpu* cpu) {
    cpu->interruption_master_enable = false;
}

void xor_instruction(Cpu* cpu) {
    cpu->regs.a ^= cpu->current_data && 0xff;
    cpu->regs.f = 0;
    cpu->regs.f_z = !!cpu->regs.a;
}


void pop_instruction(Cpu* cpu) {
    uint16_t lo = stack_pop(cpu);
    cpu->cycles++;
    uint16_t hi = stack_pop(cpu);
    cpu->cycles++;
    uint16_t value = (hi << 8) | lo;
    set_reg(cpu, cpu->current_instruction->reg_a, value);
    if (cpu->current_instruction->reg_a == REGISTER_NAME_AF) {
        set_reg(cpu, cpu->current_instruction->reg_a, value & 0xfff0);
    }
}

void push_instruction(Cpu* cpu) {
    uint16_t hi = (get_reg(cpu, cpu->current_instruction->reg_a) >> 8) & 0xff;
    cpu->cycles++;
    stack_push(cpu, hi);
    uint16_t lo = get_reg(cpu, cpu->current_instruction->reg_a) & 0xff;
    cpu->cycles++;
    stack_push(cpu, lo);
    cpu->cycles++;
}
