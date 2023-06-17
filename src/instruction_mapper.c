#include "cpu.h"
#include "instruction.h"
#include "instruction_functions.h"

Instruction instructions[0x100] = {
    [0x00] = {INSTRUCTION_NOP, ADDRESSING_MODE_NONE},
    [0x01] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_BC},
    [0x02] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R, REGISTER_NAME_BC, REGISTER_NAME_A},
    [0x03] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_BC},
    [0x04] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_B},
    [0x05] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_B},

    [0x06] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_B},
    [0x07] = {INSTRUCTION_RLCA},
    [0x08] = {INSTRUCTION_LD, ADDRESSING_MODE_A16_R, REGISTER_NAME_NONE, REGISTER_NAME_SP},
    [0x09] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_BC},
    [0x0A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_BC},
    [0x0B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_BC},
    [0x0C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_C},
    [0x0D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_C},
    [0x0E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_C},
    [0x0F] = {INSTRUCTION_RRCA},

    [0x10] = {INSTRUCTION_STOP},
    [0x11] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_DE},
    [0x12] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R, REGISTER_NAME_DE, REGISTER_NAME_A},
    [0x13] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_DE},
    [0x14] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0x15] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0x16] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_D},
    [0x17] = {INSTRUCTION_RLA},
    [0x18] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8},
    [0x19] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_DE},
    [0x1A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_DE},
    [0x1B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_DE},
    [0x1C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0x1D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0x1E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8, REGISTER_NAME_E},
    [0x1F] = {INSTRUCTION_RRA},

    [0x20] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0x21] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_HL},
    [0x22] = {INSTRUCTION_LD, ADDRESSING_MODE_HLI_R,  REGISTER_NAME_HL, REGISTER_NAME_A},
    [0x23] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_HL},
    [0x24] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_H},
    [0x25] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_H},
    [0x26] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_H},
    [0x27] = {INSTRUCTION_DAA},
    [0x28] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0x29] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_HL},
    [0x2A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_HLI,  REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x2B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_HL},
    [0x2C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_L},
    [0x2D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_L},
    [0x2E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_L},
    [0x2F] = {INSTRUCTION_CPL},

    [0x30] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0x31] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D16, REGISTER_NAME_SP},
    [0x32] = {INSTRUCTION_LD, ADDRESSING_MODE_HLD_R,  REGISTER_NAME_HL, REGISTER_NAME_A},
    [0x33] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_SP},
    [0x34] = {INSTRUCTION_INC, ADDRESSING_MODE_MR, REGISTER_NAME_HL},
    [0x35] = {INSTRUCTION_DEC, ADDRESSING_MODE_MR,    REGISTER_NAME_HL},
    [0x36] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_D8,  REGISTER_NAME_HL},
    [0x37] = {INSTRUCTION_SCF},
    [0x38] = {INSTRUCTION_JR, ADDRESSING_MODE_IMM8, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0x39] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_HL, REGISTER_NAME_SP},
    [0x3A] = {INSTRUCTION_LD, ADDRESSING_MODE_R_HLD,  REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x3B] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_SP},
    [0x3C] = {INSTRUCTION_INC, ADDRESSING_MODE_R, REGISTER_NAME_A},
    [0x3D] = {INSTRUCTION_DEC, ADDRESSING_MODE_R,    REGISTER_NAME_A},
    [0x3E] = {INSTRUCTION_LD, ADDRESSING_MODE_R_D8,  REGISTER_NAME_A},
    [0x3F] = {INSTRUCTION_CCF},

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

    [0x80] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_B},
    [0x81] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_C},
    [0x82] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_D},
    [0x83] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_E},
    [0x84] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_H},
    [0x85] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_L},
    [0x86] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x87] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_A},
    [0x88] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_B},
    [0x89] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_C},
    [0x8A] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_D},
    [0x8B] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_E},
    [0x8c] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_H},
    [0x8D] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_L},
    [0x8E] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x8F] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_A},

    [0x90] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_B},
    [0x91] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_C},
    [0x92] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0x93] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0x94] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_H},
    [0x95] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_L},
    [0x96] = {INSTRUCTION_SUB, ADDRESSING_MODE_MR, REGISTER_NAME_HL},
    [0x97] = {INSTRUCTION_SUB, ADDRESSING_MODE_R, REGISTER_NAME_A},
    [0x98] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_B},
    [0x99] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_C},
    [0x9a] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_D},
    [0x9b] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_E},
    [0x9c] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_H},
    [0x9d] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_L},
    [0x9e] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_HL},
    [0x9f] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_A},

    [0xa0] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_B},
    [0xa1] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_C},
    [0xa2] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0xa3] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0xa4] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_H},
    [0xa5] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_L},
    [0xa6] = {INSTRUCTION_AND, ADDRESSING_MODE_MR, REGISTER_NAME_HL},
    [0xa7] = {INSTRUCTION_AND, ADDRESSING_MODE_R, REGISTER_NAME_A},
    [0xa8] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_B},
    [0xa9] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_C},
    [0xaa] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_D},
    [0xab] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_E},
    [0xac] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_H},
    [0xad] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_L},
    [0xae] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_HL},
    [0xaf] = {INSTRUCTION_XOR, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_A},

    [0xb0] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_B},
    [0xb1] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_C},
    [0xb2] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_D},
    [0xb3] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_E},
    [0xb4] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_H},
    [0xb5] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_L},
    [0xb6] = {INSTRUCTION_OR, ADDRESSING_MODE_MR, REGISTER_NAME_HL},
    [0xb7] = {INSTRUCTION_OR, ADDRESSING_MODE_R, REGISTER_NAME_A},
    [0xb8] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_B},
    [0xb9] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_C},
    [0xba] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_D},
    [0xbb] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_E},
    [0xbc] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_H},
    [0xbd] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_L},
    [0xbe] = {INSTRUCTION_CP, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_HL},
    [0xbf] = {INSTRUCTION_CP, ADDRESSING_MODE_R_R, REGISTER_NAME_A, REGISTER_NAME_A},

    [0xC0] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0xC1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_BC},
    [0xC2] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0xC3] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16},
    [0xC4] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NZ},
    [0xC5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_BC},
    [0xC6] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_A8, REGISTER_NAME_A},
    [0xC7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x0},
    [0xC8] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0xC9] = {INSTRUCTION_RET},
    [0xCA] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0xCB] = {INSTRUCTION_CB, ADDRESSING_MODE_IMM8},
    [0xCC] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_Z},
    [0xCE] = {INSTRUCTION_ADC, ADDRESSING_MODE_R_A8, REGISTER_NAME_A},
    [0xCD] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16},
    [0xCF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x0f},

    [0xD0] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0xD1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_DE},
    [0xD2] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0xD4] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NC},
    [0xD5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_DE},
    [0xD6] = {INSTRUCTION_SUB, ADDRESSING_MODE_A8},
    [0xD7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x10},
    [0xD8] = {INSTRUCTION_RET, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0xD9] = {INSTRUCTION_RETI},
    [0xDA] = {INSTRUCTION_JP, ADDRESSING_MODE_IMM16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0xDC] = {INSTRUCTION_CALL, ADDRESSING_MODE_A16, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_C},
    [0xDE] = {INSTRUCTION_SBC, ADDRESSING_MODE_R_A8, REGISTER_NAME_A},
    [0xDF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x18},

    [0xE0] = {INSTRUCTION_LDH, ADDRESSING_MODE_A8_R, REGISTER_NAME_NONE, REGISTER_NAME_A},
    [0xE1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_HL},
    [0xE2] = {INSTRUCTION_LD, ADDRESSING_MODE_MR_R, REGISTER_NAME_C, REGISTER_NAME_A},
    [0xE5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_HL},
    [0xE6] = {INSTRUCTION_AND, ADDRESSING_MODE_A8},
    [0xE7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x20},
    [0xE8] = {INSTRUCTION_ADD, ADDRESSING_MODE_R_A8, REGISTER_NAME_SP},
    [0xE9] = {INSTRUCTION_JP, ADDRESSING_MODE_R, REGISTER_NAME_HL},
    [0xEA] = {INSTRUCTION_LD, ADDRESSING_MODE_A16_R, REGISTER_NAME_NONE, REGISTER_NAME_A},
    [0xEE] = {INSTRUCTION_XOR, ADDRESSING_MODE_A8},
    [0xEF] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x28},

    [0xF0] = {INSTRUCTION_LDH, ADDRESSING_MODE_R_A8, REGISTER_NAME_A, REGISTER_NAME_NONE},
    [0xF1] = {INSTRUCTION_POP, ADDRESSING_MODE_NONE, REGISTER_NAME_AF},
    [0xF2] = {INSTRUCTION_LD, ADDRESSING_MODE_R_MR, REGISTER_NAME_A, REGISTER_NAME_C},
    [0xF3] = {INSTRUCTION_DI},
    [0xF5] = {INSTRUCTION_PUSH, ADDRESSING_MODE_R, REGISTER_NAME_AF},
    [0xF6] = {INSTRUCTION_OR, ADDRESSING_MODE_A8},
    [0xF7] = {INSTRUCTION_RST, ADDRESSING_MODE_NONE, REGISTER_NAME_NONE, REGISTER_NAME_NONE, CONDITION_FLAG_NONE, 0x30},
    [0xF8] = {INSTRUCTION_LD, ADDRESSING_MODE_HL_SPR, REGISTER_NAME_HL, REGISTER_NAME_SP},
    [0xF9] = {INSTRUCTION_LD, ADDRESSING_MODE_R_R, REGISTER_NAME_SP, REGISTER_NAME_HL},
    [0xFA] = {INSTRUCTION_LD, ADDRESSING_MODE_R_A16, REGISTER_NAME_A},
    [0xFB] = {INSTRUCTION_EI},
    [0xFE] = {INSTRUCTION_CP, ADDRESSING_MODE_A8},
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
    [INSTRUCTION_EI] = ei_instruction,
    [INSTRUCTION_POP] = pop_instruction,
    [INSTRUCTION_PUSH] = push_instruction,
    [INSTRUCTION_CALL] = call_instruction,
    [INSTRUCTION_RET] = ret_instruction,
    [INSTRUCTION_RETI] = reti_instruction,
    [INSTRUCTION_RST] = rst_instruction,
    [INSTRUCTION_INC] = inc_instruction,
    [INSTRUCTION_DEC] = dec_instruction,
    [INSTRUCTION_ADD] = add_instruction,
    [INSTRUCTION_ADC] = adc_instruction,
    [INSTRUCTION_SUB] = sub_instruction,
    [INSTRUCTION_SBC] = sbc_instruction,
    [INSTRUCTION_AND] = and_instruction,
    [INSTRUCTION_XOR] = xor_instruction,
    [INSTRUCTION_OR] = or_instruction,
    [INSTRUCTION_CP] = cp_instruction,
    [INSTRUCTION_CB] = cb_instruction,

    [INSTRUCTION_RLCA] = rlca_instruction,
    [INSTRUCTION_RRCA] = rrca_instruction,
    [INSTRUCTION_RLA] = rla_instruction,
    [INSTRUCTION_RRA] = rra_instruction,
    [INSTRUCTION_DAA] = daa_instruction,
    [INSTRUCTION_CPL] = cpl_instruction,
    [INSTRUCTION_SCF] = scf_instruction,
    [INSTRUCTION_CCF] = ccf_instruction,
};
