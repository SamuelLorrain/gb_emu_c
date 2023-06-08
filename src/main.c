#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char **argv) {
    Registers regs;
    Registers_debug(regs);
    return 0;
}
