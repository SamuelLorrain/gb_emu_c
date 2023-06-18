#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdint.h>
#include "cartridge.h"
#include "cpu.h"
#include "instruction.h"

# define DEBUG_INSTRUCTION_BUFFER_SIZE 0x100

void debug_instruction(char* buffer, uint8_t const rom[], uint16_t const pc);
void debug_cb_instruction(FILE* stream, uint8_t const rom[], unsigned int const pc);
void debug_registers(CpuRegisters* const regs);
void debug_registers_inline(CpuRegisters* const regs);
void debug_header(RomHeader* const rom_header, uint8_t const rom[static 0x150]);
void debug_flag_inline(CpuRegisters* const regs);

#endif
