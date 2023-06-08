#ifndef DEBUGGER_H
#define DEBUGGER_H


enum Instruction {
    LD,
    PUSH,
    POP,

    ADD,
    ADC,
    SUB,
    SBC,
    AND,
    XOR,
    OR,
    CP,
    INC,
    DEC,
    DAA,
    CPL,

    RLCA,
    RLA,
    RRCA,
    RRA,
    RLC,
    RL,
    RRC,
    RR,
    SLA,
    SWAP,
    SRA,
    SRL,

    BIT,
    SET,
    RES,

    NOP,
    STOP,
    CCF,
    SCF,
    HALT,
    DI,
    DE,

    JR,
    JP,
    RET,
    CALL,
    RETI,
    RST
}

enum RegisterEnum {
    A,
    B,
    C,
    D,
    E,
    F,
    H,
    L,

    AF,
    BC,
    DE,
    HL,

    SP,
    PC
}

enum 


typedef struct {
    Instruction ins;
    Operand op1;
    Operand op2;
    uint16_t opcode;
    uint8_t clock_cycles;
} Debugger;

#endif
