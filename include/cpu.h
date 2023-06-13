#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include "mmu.h"
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
    uint8_t current_opcode;
    uint16_t current_data;
    uint16_t current_memory_destination;
    uint16_t current_destination_in_memory;
    Instruction* current_instruction;
    uint8_t ei_register;
    bool interruption_master_enable;
    uint64_t cycles;
    CpuRegisters regs;
    MMU mmu;
} Cpu;

void step(Cpu* cpu);
uint16_t get_reg(Cpu* cpu, RegisterName reg_name);
void set_reg(Cpu* cpu, RegisterName reg_name, uint16_t value);

uint8_t mmu_read(Cpu* cpu, uint16_t addr);
uint16_t mmu_read16(Cpu* cpu, uint16_t addr);
void mmu_write(Cpu* cpu, uint16_t addr, uint8_t value);
void mmu_write16(Cpu* cpu, uint16_t addr, uint16_t value);

void stack_push(Cpu* cpu, uint8_t value);
void stack_push16(Cpu* cpu, uint16_t value);
uint8_t stack_pop(Cpu* cpu);
uint16_t stack_pop16(Cpu* cpu);

#endif
