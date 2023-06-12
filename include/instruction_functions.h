#ifndef INSTRUCTION_FUNCTIONS_H
#define INSTRUCTION_FUNCTIONS_H
#include "cpu.h"

typedef void cpu_instruction_function_ptr(Cpu*);

bool check_condition(Cpu* cpu);
void invalid_instruction(Cpu* cpu);
void ld_instruction(Cpu* cpu);
void jp_instruction(Cpu* cpu);
void jr_instruction(Cpu* cpu);
void nop_instruction();
void di_instruction(Cpu* cpu);

#endif
