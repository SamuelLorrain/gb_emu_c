#ifndef INSTRUCTION_FUNCTIONS_H
#define INSTRUCTION_FUNCTIONS_H
#include "cpu.h"

typedef void cpu_instruction_function_ptr(Cpu*);

bool check_condition(Cpu* cpu);
void invalid_instruction(Cpu* cpu);
void ld_instruction(Cpu* cpu);
void ldh_instruction(Cpu* cpu);
void jp_instruction(Cpu* cpu);
void jr_instruction(Cpu* cpu);
void nop_instruction();
void di_instruction(Cpu* cpu);
void ei_instruction(Cpu* cpu);
void pop_instruction(Cpu* cpu);
void push_instruction(Cpu* cpu);
void call_instruction(Cpu* cpu);
void ret_instruction(Cpu* cpu);
void reti_instruction(Cpu* cpu);
void rst_instruction(Cpu* cpu);
void inc_instruction(Cpu* cpu);
void dec_instruction(Cpu* cpu);
void add_instruction(Cpu* cpu);
void adc_instruction(Cpu* cpu);
void sub_instruction(Cpu* cpu);
void sbc_instruction(Cpu* cpu);
void and_instruction(Cpu* cpu);
void xor_instruction(Cpu* cpu);
void or_instruction(Cpu* cpu);
void cp_instruction(Cpu* cpu);
void cb_instruction(Cpu* cpu);

void rlc_instruction(Cpu* cpu, int reg_name);
void rrc_instruction(Cpu* cpu, int reg_name);
void rr_instruction(Cpu* cpu, int reg_name);
void rl_instruction(Cpu* cpu, int reg_name);
void sla_instruction(Cpu* cpu, int reg_name);
void sra_instruction(Cpu* cpu, int reg_name);
void swap_instruction(Cpu* cpu, int reg_name);
void srl_instruction(Cpu* cpu, int reg_name);
void bit_instruction(Cpu* cpu, int reg_name, uint8_t bit);
void resset_instruction(Cpu* cpu, int reg_name, uint8_t bit, uint8_t set_or_reset);

void rlca_instruction(Cpu* cpu);
void rrca_instruction(Cpu* cpu);
void rla_instruction(Cpu* cpu);
void rra_instruction(Cpu* cpu);
void daa_instruction(Cpu* cpu);
void cpl_instruction(Cpu* cpu);
void scf_instruction(Cpu* cpu);
void ccf_instruction(Cpu* cpu);

void halt_instruction(Cpu* cpu);
void stop_instruction(Cpu* cpu);

#endif
