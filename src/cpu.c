#include "cpu.h"
#include <stdio.h>

int fetchCurrent(Cpu* c) {
    return c->ram[c->reg.pc];
}

int fetchNext(Cpu* c) {
    c->reg.pc += 1;
    return c->ram[c->reg.pc];
}

