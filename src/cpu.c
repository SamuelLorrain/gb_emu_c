#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"

/* Jump conditions maybe enum instead of macro */
#define NZ 0x1
#define NC 0x2
#define Z 0x3
#define C 0x4
#define NO_CONDITION 0x5

#define SET_F_Z(v) ((v == 0) ? true : false)
#define GET_IMM16(ram, pc) (((ram)[(pc)+2] << 8) + (ram)[(pc)+1])
#define LD_IMM16(ram, regs, x) (regs->x = GET_IMM16(ram, regs->pc))

#define GET_IMM8(ram, pc) ((ram)[(pc)+1])
#define LD_IMM8(ram, regs, x) (regs->x = GET_IMM8(ram, regs->pc))
#define LD_X8_Y8(regs, x, y) (regs->x = regs->y)

#define INC_X8(regs, cur_reg) {\
            regs->f_h = (regs->cur_reg & 0x0f) + 1 > 0x0f; \
            regs->cur_reg += 1; \
            regs->f_z = SET_F_Z(regs->cur_reg); \
            regs->f_n = false; \
        }
#define DEC_X8(regs, cur_reg) {\
            regs->b--; \
            regs->f_z = SET_F_Z(regs->b); \
            regs->f_h = (regs->b & 0x0f) == 0; \
            regs->f_n = true; \
        }
#define JR_S8(condition, ram, regs) {\
            if ((condition == NZ) && regs->f_z == 0) {\
                regs->pc += GET_IMM8(ram, regs->pc); \
            } else if ((condition == NC) && regs->f_c == 0) {\
                regs->pc += GET_IMM8(ram, regs->pc); \
            } else if ((condition == Z) && regs->f_z == 1) {\
                regs->pc += GET_IMM8(ram, regs->pc); \
            } else if ((condition == C) && regs->f_c == 1) {\
                regs->pc += GET_IMM8(ram, regs->pc); \
            } else if ((condition == NO_CONDITION)) { \
                regs->pc += GET_IMM8(ram, regs->pc); \
            } else { \
                regs->pc++; \
            }\
        }

#define ADD_A_Y8(regs, two) {\
            regs->f_c = (regs->a + regs->two > 0xff);\
            regs->f_n = 0;\
            regs->f_h = ((regs->a & 0xf) + (regs->two & 0xf)) > 0xf;\
            regs->a += regs->two;\
            regs->f_z = SET_F_Z(regs->a);\
        }

#define ADC_A_Y8(regs, two) {\
            regs->f_c = ((regs->a + regs->two + regs->f_c) > 0xff);\
            regs->f_n = 0;\
            regs->f_h = ((regs->a & 0xf) + (regs->two & 0xf) + regs->f_c) > 0xf;\
            regs->a += regs->two + regs->f_c;\
            regs->f_z = SET_F_Z(regs->a);\
        }

#define SUB_A_Y8(regs, two) {\
            regs->f_c = (regs->a - regs->two > 0xff);\
            regs->f_n = 1;\
            regs->f_h = ((regs->a & 0xf) - (regs->two & 0xf)) > 0xf;\
            regs->a -= regs->two;\
            regs->f_z = SET_F_Z(regs->a);\
        }

#define SBC_A_Y8(regs, two) {\
            regs->f_c = ((regs->a - regs->two - regs->f_c) > 0xff);\
            regs->f_n = 1;\
            regs->f_h = ((regs->a & 0xf) - (regs->two & 0xf) - regs->f_c) > 0xf;\
            regs->a -= regs->two - regs->f_c;\
            regs->f_z = SET_F_Z(regs->a);\
        }

void debug_registers(Registers regs) {
    printf("a = 0x%2x\n", regs.a);
    printf("b = 0x%2x\n", regs.b);
    printf("c = 0x%2x\n", regs.c);
    printf("d = 0x%2x\n", regs.d);
    printf("e = 0x%2x\n", regs.e);
    printf("h = 0x%2x\n", regs.h);
    printf("l = 0x%2x\n", regs.l);
    printf("f = 0x%2x\n", regs.f);

    printf("=======\n");

    printf("af = 0x%4x\n", regs.af);
    printf("bc = 0x%4x\n", regs.bc);
    printf("de = 0x%4x\n", regs.de);
    printf("hl = 0x%4x\n", regs.hl);
    printf("sp = 0x%4x\n", regs.sp);
    printf("pc = 0x%4x\n", regs.pc);

    printf("=======\n");
    printf("FLAGS:\n");
    printf("c, h, n, z\n");
    printf("%1d, %1d, %1d, %1d\n", regs.f_c, regs.f_h, regs.f_n, regs.f_z);
}

void execute_next(Registers* regs, unsigned char ram[static 0xffff]) {
    unsigned char current = ram[regs->pc];
    bool flag_c = 0;
    /* printf("PC: 0x%x, %u\n", regs->pc, regs->pc); */
    switch(current) {
        case 0x00: regs->pc++; break; // NOP
        case 0x76: regs->pc++; break; // STOP
        case 0x10: regs->pc++; break; // HALT
        case 0x01: regs->bc = GET_IMM16(ram, regs->pc); regs->pc += 3; break;
        case 0x11: regs->de = GET_IMM16(ram, regs->pc); regs->pc += 3; break;
        case 0x21: regs->hl = GET_IMM16(ram, regs->pc); regs->pc += 3; break;
        case 0x31: regs->sp = GET_IMM16(ram, regs->pc); regs->pc += 3; break;

        case 0x02: ram[regs->bc] = regs->a; regs->pc++; break;
        case 0x12: ram[regs->de] = regs->a; regs->pc++; break;

        case 0x0a: regs->a = ram[regs->bc]; regs->pc++; break;
        case 0x1a: regs->a = ram[regs->de]; regs->pc++; break;

        case 0x46: regs->b = ram[regs->hl]; regs->pc++; break;
        case 0x56: regs->d = ram[regs->hl]; regs->pc++; break;
        case 0x66: regs->h = ram[regs->hl]; regs->pc++; break;
        case 0x4e: regs->c = ram[regs->hl]; regs->pc++; break;
        case 0x5e: regs->e = ram[regs->hl]; regs->pc++; break;
        case 0x6e: regs->e = ram[regs->hl]; regs->pc++; break;
        case 0x7e: regs->a = ram[regs->hl]; regs->pc++; break;
        case 0x70: ram[regs->hl] = regs->b; regs->pc++; break;
        case 0x71: ram[regs->hl] = regs->c; regs->pc++; break;
        case 0x72: ram[regs->hl] = regs->d; regs->pc++; break;
        case 0x73: ram[regs->hl] = regs->e; regs->pc++; break;
        case 0x74: ram[regs->hl] = regs->h; regs->pc++; break;
        case 0x75: ram[regs->hl] = regs->l; regs->pc++; break;
        case 0x77: ram[regs->hl] = regs->a; regs->pc++; break;

        case 0x03: regs->bc++; regs->pc++; break;
        case 0x13: regs->de++; regs->pc++; break;
        case 0x23: regs->hl++; regs->pc++; break;
        case 0x33: regs->sp++; regs->pc++; break;
        case 0x0b: regs->bc--; regs->pc++; break;
        case 0x1b: regs->de--; regs->pc++; break;
        case 0x2b: regs->hl--; regs->pc++; break;
        case 0x3b: regs->sp--; regs->pc++; break;

        case 0x04: INC_X8(regs, b); regs->pc++; break;
        case 0x0c: INC_X8(regs, c); regs->pc++; break;
        case 0x14: INC_X8(regs, d); regs->pc++; break;
        case 0x1c: INC_X8(regs, e); regs->pc++; break;
        case 0x24: INC_X8(regs, h); regs->pc++; break;
        case 0x2c: INC_X8(regs, l); regs->pc++; break;
        case 0x3c: INC_X8(regs, a); regs->pc++; break;

        case 0x05: DEC_X8(regs, b); regs->pc++; break;
        case 0x0d: DEC_X8(regs, c); regs->pc++; break;
        case 0x15: DEC_X8(regs, d); regs->pc++; break;
        case 0x1d: DEC_X8(regs, e); regs->pc++; break;
        case 0x25: DEC_X8(regs, h); regs->pc++; break;
        case 0x2d: DEC_X8(regs, l); regs->pc++; break;
        case 0x3d: DEC_X8(regs, a); regs->pc++; break;

        case 0x07:
            regs->f = 0;
            regs->f_c = regs->a >> 7;
            regs->a = (regs->a << 1) | regs->f_c;
            regs->pc++;
            break;
        case 0x17:
            flag_c = regs->f_c;
            regs->f = 0;
            regs->f_c = regs->a >> 7;
            regs->a = (regs->a << 1) | flag_c;
            regs->pc++;
            break;
        case 0x0f:
            regs->f = 0;
            regs->f_c = regs->a & 0x1;
            regs->a = (regs->a >> 1) | (regs->f_c << 7);
            regs->pc++;
            break;
        case 0x1f:
            flag_c = regs->f_c;
            regs->f = 0;
            regs->f_c = regs->a & 0x1;
            regs->a = (regs->a >> 1) | (flag_c << 7);
            regs->pc++;
            break;

        case 0x08:
            ram[GET_IMM16(ram, regs->pc)] = regs->sp & 0xff;
            ram[GET_IMM16(ram, regs->pc) + 1] = regs->sp >> 8;
            regs->pc += 3;
            break;

        case 0x06: LD_IMM8(ram, regs, b); regs->pc += 2; break;
        case 0x16: LD_IMM8(ram, regs, d); regs->pc += 2; break;
        case 0x26: LD_IMM8(ram, regs, h); regs->pc += 2; break;
        case 0x0e: LD_IMM8(ram, regs, c); regs->pc += 2; break;
        case 0x1e: LD_IMM8(ram, regs, e); regs->pc += 2; break;
        case 0x2e: LD_IMM8(ram, regs, l); regs->pc += 2; break;
        case 0x3e: LD_IMM8(ram, regs, a); regs->pc += 2; break;

        case 0x40: LD_X8_Y8(regs, b, b); regs->pc++; break;
        case 0x41: LD_X8_Y8(regs, b, c); regs->pc++; break;
        case 0x42: LD_X8_Y8(regs, b, d); regs->pc++; break;
        case 0x43: LD_X8_Y8(regs, b, e); regs->pc++; break;
        case 0x44: LD_X8_Y8(regs, b, h); regs->pc++; break;
        case 0x45: LD_X8_Y8(regs, b, l); regs->pc++; break;
        /* case 0x46: LD_X8_Y8(raregs, m, b, b); regs->pc++; break; */
        case 0x47: LD_X8_Y8(regs, b, a); regs->pc++; break;
        case 0x48: LD_X8_Y8(regs, c, b); regs->pc++; break;
        case 0x49: LD_X8_Y8(regs, c, c); regs->pc++; break;
        case 0x4a: LD_X8_Y8(regs, c, d); regs->pc++; break;
        case 0x4b: LD_X8_Y8(regs, c, e); regs->pc++; break;
        case 0x4c: LD_X8_Y8(regs, c, h); regs->pc++; break;
        case 0x4d: LD_X8_Y8(regs, c, l); regs->pc++; break;
        /* case 0x4e: LD_X8_Y8(ram, c, b); regs->pc++; break; */
        case 0x4f: LD_X8_Y8(regs, c, a); regs->pc++; break;

        case 0x50: LD_X8_Y8(regs, d, b); regs->pc++; break;
        case 0x51: LD_X8_Y8(regs, d, c); regs->pc++; break;
        case 0x52: LD_X8_Y8(regs, d, d); regs->pc++; break;
        case 0x53: LD_X8_Y8(regs, d, e); regs->pc++; break;
        case 0x54: LD_X8_Y8(regs, d, h); regs->pc++; break;
        case 0x55: LD_X8_Y8(regs, d, l); regs->pc++; break;
        /* case 0x46: LD_X8_Y8(raregs, m, d, b); regs->pc++; break; */
        case 0x57: LD_X8_Y8(regs, d, a); regs->pc++; break;
        case 0x58: LD_X8_Y8(regs, e, b); regs->pc++; break;
        case 0x59: LD_X8_Y8(regs, e, c); regs->pc++; break;
        case 0x5a: LD_X8_Y8(regs, e, d); regs->pc++; break;
        case 0x5b: LD_X8_Y8(regs, e, e); regs->pc++; break;
        case 0x5c: LD_X8_Y8(regs, e, h); regs->pc++; break;
        case 0x5d: LD_X8_Y8(regs, e, l); regs->pc++; break;
        /* case 0x4e: LD_X8_Y8(ram, e, b); regs->pc++; break; */
        case 0x5f: LD_X8_Y8(regs, e, a); regs->pc++; break;

        case 0x60: LD_X8_Y8(regs, h, b); regs->pc++; break;
        case 0x61: LD_X8_Y8(regs, h, c); regs->pc++; break;
        case 0x62: LD_X8_Y8(regs, h, d); regs->pc++; break;
        case 0x63: LD_X8_Y8(regs, h, e); regs->pc++; break;
        case 0x64: LD_X8_Y8(regs, h, h); regs->pc++; break;
        case 0x65: LD_X8_Y8(regs, h, l); regs->pc++; break;
        /* case 0x46: LD_X8_Y8(raregs, m, d, b); regs->pc++; break; */
        case 0x67: LD_X8_Y8(regs, h, a); regs->pc++; break;
        case 0x68: LD_X8_Y8(regs, l, b); regs->pc++; break;
        case 0x69: LD_X8_Y8(regs, l, c); regs->pc++; break;
        case 0x6a: LD_X8_Y8(regs, l, d); regs->pc++; break;
        case 0x6b: LD_X8_Y8(regs, l, e); regs->pc++; break;
        case 0x6c: LD_X8_Y8(regs, l, h); regs->pc++; break;
        case 0x6d: LD_X8_Y8(regs, l, l); regs->pc++; break;
        /* case 0x4e: LD_X8_Y8(ram, e, b); regs->pc++; break; */
        case 0x6f: LD_X8_Y8(regs, l, a); regs->pc++; break;

        case 0x78: LD_X8_Y8(regs, a, b); regs->pc++; break;
        case 0x79: LD_X8_Y8(regs, a, c); regs->pc++; break;
        case 0x7a: LD_X8_Y8(regs, a, d); regs->pc++; break;
        case 0x7b: LD_X8_Y8(regs, a, e); regs->pc++; break;
        case 0x7c: LD_X8_Y8(regs, a, h); regs->pc++; break;
        case 0x7d: LD_X8_Y8(regs, a, l); regs->pc++; break;
        /* case 0x4e: LD_X8_Y8(ram, e, b); regs->pc++; break; */
        case 0x7f: LD_X8_Y8(regs, a, a); regs->pc++; break;

        case 0x20: JR_S8(NZ, ram, regs); break;
        case 0x30: JR_S8(NC, ram, regs); break;
        case 0x18: JR_S8(NO_CONDITION, ram, regs); break;
        case 0x28: JR_S8(Z, ram, regs); break;
        case 0x38: JR_S8(C, ram, regs); break;

        case 0x80: ADD_A_Y8(regs, b); regs->pc++; break;
        case 0x81: ADD_A_Y8(regs, c); regs->pc++; break;
        case 0x82: ADD_A_Y8(regs, d); regs->pc++; break;
        case 0x83: ADD_A_Y8(regs, e); regs->pc++; break;
        case 0x84: ADD_A_Y8(regs, h); regs->pc++; break;
        case 0x85: ADD_A_Y8(regs, l); regs->pc++; break;
        case 0x87: ADD_A_Y8(regs, a); regs->pc++; break;

        case 0x88: ADC_A_Y8(regs, b); regs->pc++; break;
        case 0x89: ADC_A_Y8(regs, c); regs->pc++; break;
        case 0x8a: ADC_A_Y8(regs, d); regs->pc++; break;
        case 0x8b: ADC_A_Y8(regs, e); regs->pc++; break;
        case 0x8c: ADC_A_Y8(regs, h); regs->pc++; break;
        case 0x8d: ADC_A_Y8(regs, l); regs->pc++; break;
        case 0x8f: ADC_A_Y8(regs, a); regs->pc++; break;

        case 0x90: SUB_A_Y8(regs, b); regs->pc++; break;
        case 0x91: SUB_A_Y8(regs, c); regs->pc++; break;
        case 0x92: SUB_A_Y8(regs, d); regs->pc++; break;
        case 0x93: SUB_A_Y8(regs, e); regs->pc++; break;
        case 0x94: SUB_A_Y8(regs, h); regs->pc++; break;
        case 0x95: SUB_A_Y8(regs, l); regs->pc++; break;
        case 0x97: SUB_A_Y8(regs, a); regs->pc++; break;

        case 0x98: SBC_A_Y8(regs, b); regs->pc++; break;
        case 0x99: SBC_A_Y8(regs, c); regs->pc++; break;
        case 0x9a: SBC_A_Y8(regs, d); regs->pc++; break;
        case 0x9b: SBC_A_Y8(regs, e); regs->pc++; break;
        case 0x9c: SBC_A_Y8(regs, h); regs->pc++; break;
        case 0x9d: SBC_A_Y8(regs, l); regs->pc++; break;
        case 0x9f: SBC_A_Y8(regs, a); regs->pc++; break;
        default:
            /* fprintf(stderr, "Non implemented opcode %02x\n", current); */
            printf("%02x\n", current);
            /* exit(1); */
    }
}

