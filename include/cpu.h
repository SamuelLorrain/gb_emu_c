#ifndef CPU_H
#define CPU_H
#include <stdint.h>

typedef struct {
    union {
        uint16_t af;
        struct {
            struct {
                uint8_t: 4;
                uint8_t f_c: 1;
                uint8_t f_h: 1;
                uint8_t f_n: 1;
                uint8_t f_z: 1;
            };
            uint8_t a;
        };
    };
    union {
        uint16_t bc;
        struct {
            uint8_t b;
            uint8_t c;
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
} Registers;

struct {
    Registers regs;
    uint8_t* ram;
} Cpu;

void debugRegisters(Registers regs);
int fetchCurrent(cpu* c);
int fetchNext(cpu* c);

#endif
