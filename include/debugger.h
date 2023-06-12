#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdint.h>
#include "cartridge.h"
#include "cpu.h"
#include "instructions_debugger.h"

void debug_instructions(uint8_t const rom[], uint16_t const offset, size_t const nb_instructions);
void debug_cb_instruction(uint8_t const rom[], unsigned int const pc);
void debug_registers(Registers* const regs);
void debug_header(RomHeader* const rom_header, uint8_t const rom[static 0x150]);

#endif
