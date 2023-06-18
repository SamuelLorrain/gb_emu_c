#include "cpu.h"
#include "interrupt.h"
#include "stack_operations.h"

void call_handle_interrupt(Cpu* cpu, uint16_t address) {
    stack_push16(cpu, cpu->regs.pc);
    cpu->regs.pc = address;
}

void handle_interrupt(Cpu* cpu) {
    uint16_t address = -1;
    uint8_t ei_register = cpu->mmu.ei_register;
    if ((ei_register & INTERRUPTION_VBLANK) == INTERRUPTION_VBLANK) {
        address = 0x40;
    } else if ((ei_register & INTERRUPTION_LCD_STAT) == INTERRUPTION_LCD_STAT) {
        address = 0x48;
    } else if ((ei_register & INTERRUPTION_TIMER) == INTERRUPTION_TIMER) {
        address = 0x50;
    } else if ((ei_register & INTERRUPTION_SERIAL) == INTERRUPTION_SERIAL) {
        address = 0x58;
    } else if ((ei_register & INTERRUPTION_JOYPAD) == INTERRUPTION_JOYPAD) {
        address = 0x60;
    } else if (address == (uint16_t)-1) {
        return;
    }
    cpu->cycles+=2;
    cpu->mmu.ei_register = 0;
    cpu->halted = false;
    cpu->interruption_master_enable = false;
    call_handle_interrupt(cpu, address);
    cpu->cycles+=2;
}
