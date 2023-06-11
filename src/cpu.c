#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define HAS_CARRY_FLAG(oldV, v) ((((oldV & 0xf) + (v & 0xf)) & 0x10) == 0x10)

void debugRegisters(Registers regs) {
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

void execute_next(Registers* regs, unsigned char buffer[static 0xffff]) {
    unsigned char current = buffer[regs->pc];
    /* uint16_t imm16 = 0; */
    switch(current) {
        case 0x0:
            printf("NOP\n");
            regs->pc += 1;
            break;
        case 0xc3:
            regs->pc = (buffer[regs->pc+2] << 8) + buffer[regs->pc+1];
            printf("JP a16  0x%x\n", regs->pc);
            break;
        case 0x20:
            printf("JP NZ, s8\n");
            if(regs->f_z == 1) {
                regs->pc += 2;
                break;
            }
            regs->pc = buffer[regs->pc+1];
            break;
        case 0xaf:
            printf("XOR A,A\n");
            regs->a = regs->a ^ regs->a;
            regs->f = 0;
            regs->f_z = (regs->a == 0) ? 1 : 0;
            regs->pc += 1;
            break;
        case 0x21:
            printf("LD HL, d16\n");
            regs->hl = (buffer[regs->pc+2] << 8) + buffer[regs->pc+1];
            regs->pc += 3;
            break;
        case 0x0e:
            printf("LD C, d8\n");
            regs->c = buffer[regs->pc+1];
            regs->pc += 2;
            break;
        case 0x06:
            printf("LD B, d8\n");
            regs->b = buffer[regs->pc+1];
            regs->pc += 2;
            break;
        case 0x32:
            printf("LD (HL-), A\n");
            buffer[regs->hl] = regs->a;
            regs->hl -= 1;
            regs->pc += 1;
            break;
        case 0x05:
            printf("DEC B\n");
            regs->f_h = HAS_CARRY_FLAG(regs->b, (regs->b-1)) ? 1 : 0;
            regs->b -= 1;
            regs->f_z = (regs->b == 0) ? 1 : 0;
            regs->f_n = 1;
            regs->pc += 1;
            break;
        case 0xff:
            break;
        default:
            fprintf(stderr, "Unknown opcode : 0x%x, ABORT\n", current);
            exit(1);
    }
}

