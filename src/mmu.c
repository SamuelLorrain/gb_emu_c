#include <string.h>
#include "mmu.h"

void load_rom(unsigned char mmu[static 0xffff], unsigned char const rom_buffer[static 0x8000]) {
    memcpy(mmu, rom_buffer, 0x8000);
}
