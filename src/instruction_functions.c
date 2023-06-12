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
    fprintf(stderr, "NOT IMPLEMENTED");
    exit(1);
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
    cpu->interruptions = false;
}
