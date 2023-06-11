#ifndef MMU_H
#define MMU_H

void load_rom(unsigned char mmu[static 0xffff], unsigned char const rom_buffer[static 0x8000]);

#endif
