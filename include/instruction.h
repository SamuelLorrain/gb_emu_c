#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdint.h>

typedef enum {
    INSTRUCTION_NONE,
    INSTRUCTION_LD,
    INSTRUCTION_LDI,
    INSTRUCTION_LDD,
    INSTRUCTION_LDH,

    INSTRUCTION_PUSH,
    INSTRUCTION_POP,

    INSTRUCTION_ADD,
    INSTRUCTION_ADC,
    INSTRUCTION_SUB,
    INSTRUCTION_SBC,
    INSTRUCTION_AND,
    INSTRUCTION_XOR,
    INSTRUCTION_OR,
    INSTRUCTION_CP,
    INSTRUCTION_INC,
    INSTRUCTION_DEC,
    INSTRUCTION_DAA,
    INSTRUCTION_CPL,

    INSTRUCTION_RLCA,
    INSTRUCTION_RLA,
    INSTRUCTION_RRA,
    INSTRUCTION_RLC,
    INSTRUCTION_RL,
    INSTRUCTION_RRC,
    INSTRUCTION_RR,
    INSTRUCTION_SLA,
    INSTRUCTION_SWAP,
    INSTRUCTION_SRA,
    INSTRUCTION_SRL,
    INSTRUCTION_BIT,
    INSTRUCTION_SET,
    INSTRUCTION_RES,
    INSTRUCTION_CCF,
    INSTRUCTION_SCF,

    INSTRUCTION_NOP,
    INSTRUCTION_HALT,
    INSTRUCTION_STOP,

    INSTRUCTION_DI,
    INSTRUCTION_EI,
    INSTRUCTION_JP,
    INSTRUCTION_JR,
    INSTRUCTION_CALL,
    INSTRUCTION_RET,
    INSTRUCTION_RETI,
    INSTRUCTION_RST,

    INSTRUCTION_CB,
    INSTRUCTION_CB_RLC,
    INSTRUCTION_CB_RRC,
    INSTRUCTION_CB_RL,
    INSTRUCTION_CB_RR,
    INSTRUCTION_CB_SLA,
    INSTRUCTION_CB_SRA,
    INSTRUCTION_CB_SWAP,
    INSTRUCTION_CB_SRL,
    INSTRUCTION_CB_BIT,
    INSTRUCTION_CB_RES,
    INSTRUCTION_CB_SET,
} InstructionType;

typedef enum {
    REGISTER_NAME_NONE,
    REGISTER_NAME_A,
    REGISTER_NAME_B,
    REGISTER_NAME_C,
    REGISTER_NAME_D,
    REGISTER_NAME_E,
    REGISTER_NAME_F,
    REGISTER_NAME_H,
    REGISTER_NAME_L,
    REGISTER_NAME_SP,
    REGISTER_NAME_PC,
    REGISTER_NAME_AF,
    REGISTER_NAME_BC,
    REGISTER_NAME_DE,
    REGISTER_NAME_HL
} RegisterName;

typedef enum {
    ADDRESSING_MODE_NONE,
    ADDRESSING_MODE_R_D16,
    ADDRESSING_MODE_IMM16,
    ADDRESSING_MODE_A16,
    ADDRESSING_MODE_IMM8,
    ADDRESSING_MODE_R_D8,
    ADDRESSING_MODE_R,
    ADDRESSING_MODE_R_R,
    ADDRESSING_MODE_MR_R,
    ADDRESSING_MODE_R_MR,
    ADDRESSING_MODE_R_HLI,
    ADDRESSING_MODE_R_HLD,
    ADDRESSING_MODE_HLI_R,
    ADDRESSING_MODE_HLD_R,
    ADDRESSING_MODE_R_A8,
    ADDRESSING_MODE_A8_R,
    ADDRESSING_MODE_A8,
    ADDRESSING_MODE_D8,
    ADDRESSING_MODE_D16_R,
    ADDRESSING_MODE_MR_D8,
    ADDRESSING_MODE_MR,
    ADDRESSING_MODE_A16_R,
    ADDRESSING_MODE_R_A16,
    ADDRESSING_MODE_HL_SPR,
} AddressingMode;


typedef enum {
    CONDITION_FLAG_NONE,
    CONDITION_FLAG_Z,
    CONDITION_FLAG_NZ,
    CONDITION_FLAG_C,
    CONDITION_FLAG_NC,
} ConditionFlag;

typedef struct {
    InstructionType type;
    AddressingMode mode;
    RegisterName reg_a;
    RegisterName reg_b;
    ConditionFlag condition;
    uint8_t param;
} Instruction;

typedef struct {
    InstructionType type;
    uint8_t param;
} CBInstructionLayout;

#endif
