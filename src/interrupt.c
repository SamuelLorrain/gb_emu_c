#include "cpu.h"
#include "interrupt.h"

void call_handle_interrupt(Cpu* cpu, uint16_t address) {
    stack_push16(cpu->regs.pc);
    cpu->regs.pc = address;
}

void handle_interrupt(Cpu* cpu) {

}
