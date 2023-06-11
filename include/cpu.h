#ifndef CPU_H
#define CPU_H
#include <stdint.h>

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

void debug_registers(Registers regs);
void execute_next(Registers* regs, unsigned char buffer[static 0xffff]);

#endif
