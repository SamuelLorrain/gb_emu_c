#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"

void execute_next(Registers* regs, unsigned char ram[static 0xffff]) {
    unsigned char current = ram[regs->pc];
    bool flag_c = 0;
    switch(current) {
        default:
            printf("%02x unimplemented\n", current);
            exit(1);
    }
    pc+=1;
}
