#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "cpu.h"

typedef enum {
    INTERRUPTION_VBLANK = 1,
    INTERRUPTION_LCD_STAT = 2,
    INTERRUPTION_TIMER = 4,
    INTERRUPTION_SERIAL = 8,
    INTERRUPTION_JOYPAD = 16,
} InterruptionTypes;

void handle_interrupt(Cpu* cpu);

#endif
