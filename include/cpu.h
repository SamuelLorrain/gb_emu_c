#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include "instruction.h"

typedef struct {
    union {
        uint16_t af;
        struct {
            union {
                struct {
                    uint8_t: 4;
                    uint8_t f_c: 1;
                    uint8_t f_h: 1;
                    uint8_t f_n: 1;
                    uint8_t f_z: 1;
                };
                uint8_t f;
            };
            uint8_t a;
        };
    };
    union {
        uint16_t bc;
        struct {
            uint8_t c;
            uint8_t b;
        };
    };
    union {
        uint16_t de;
        struct {
            uint8_t e;
            uint8_t d;
        };
    };
    union {
        uint16_t hl;
        struct {
            uint8_t l;
            uint8_t h;
        };
    };
    uint16_t sp;
    uint16_t pc;
} CpuRegisters;

typedef struct {
    uint8_t* rom_buffer;
    uint8_t wram_buffer[0x2000];
    uint8_t hram_buffer[0x80];

    // IO registers
    uint8_t ei_register;
    uint8_t serial_data[0x2];
} MMU;

typedef struct {
    bool enabling_ime;
    bool interruption_master_enable;
    bool halted;
    uint64_t cycles;
    CpuRegisters regs;
    MMU mmu;

    // should be in a struct that act on the Cpu instead
    // of the cpu itself
    uint8_t current_opcode;
    uint16_t current_data;
    uint16_t current_memory_destination;
    uint16_t current_destination_in_memory;
    Instruction* current_instruction;
} Cpu;

void step(Cpu* cpu);
void reset_instruction_state(Cpu* cpu);
uint16_t get_reg(Cpu* cpu, RegisterName reg_name);
void set_reg(Cpu* cpu, RegisterName reg_name, uint16_t value);

#endif
