#ifndef INSTRUCTION_DEBUGGER_H
#define INSTRUCTION_DEBUGGER_H
#include "instruction.h"

static const char* instruction_type_names[] = {
    [INSTRUCTION_NONE] = "NONE" ,
    [INSTRUCTION_LD] = "LD" ,
    [INSTRUCTION_LDI] = "LDI" ,
    [INSTRUCTION_LDD] = "LDD" ,
    [INSTRUCTION_PUSH] = "PUSH" ,
    [INSTRUCTION_POP] = "POP" ,
    [INSTRUCTION_ADD] = "ADD" ,
    [INSTRUCTION_ADC] = "ADC" ,
    [INSTRUCTION_SUB] = "SUB" ,
    [INSTRUCTION_SBC] = "SBC" ,
    [INSTRUCTION_AND] = "AND" ,
    [INSTRUCTION_XOR] = "XOR" ,
    [INSTRUCTION_OR] = "OR" ,
    [INSTRUCTION_CP] = "CP" ,
    [INSTRUCTION_INC] = "INC" ,
    [INSTRUCTION_DEC] = "DEC" ,
    [INSTRUCTION_DAA] = "DAA" ,
    [INSTRUCTION_CPL] = "CPL" ,
    [INSTRUCTION_RLCA] = "RLCA" ,
    [INSTRUCTION_RLA] = "RLA" ,
    [INSTRUCTION_RRA] = "RRA" ,
    [INSTRUCTION_RLC] = "RLC" ,
    [INSTRUCTION_RL] = "RL" ,
    [INSTRUCTION_RRC] = "RRC" ,
    [INSTRUCTION_RR] = "RR" ,
    [INSTRUCTION_SLA] = "SLA" ,
    [INSTRUCTION_SWAP] = "SWAP" ,
    [INSTRUCTION_SRA] = "SRA" ,
    [INSTRUCTION_SRL] = "SRL" ,
    [INSTRUCTION_BIT] = "BIT" ,
    [INSTRUCTION_SET] = "SET" ,
    [INSTRUCTION_RES] = "RES" ,
    [INSTRUCTION_CCF] = "CCF" ,
    [INSTRUCTION_SCF] = "SCF" ,
    [INSTRUCTION_NOP] = "NOP" ,
    [INSTRUCTION_HALT] = "HALT" ,
    [INSTRUCTION_STOP] = "STOP" ,
    [INSTRUCTION_DI] = "DI" ,
    [INSTRUCTION_EI] = "EI" ,
    [INSTRUCTION_JP] = "JP" ,
    [INSTRUCTION_JR] = "JR" ,
    [INSTRUCTION_CALL] = "CALL" ,
    [INSTRUCTION_RET] = "RET" ,
    [INSTRUCTION_RETI] = "RETI" ,
    [INSTRUCTION_RST] = "RST"
};

const char* get_instruction_type_name(InstructionType instruction_type);

#endif
