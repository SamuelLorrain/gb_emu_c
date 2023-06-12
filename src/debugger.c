#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cpu.h"
#include "debugger.h"
#include "cartridge.h"

static const char* CartridgeType[] = {
    [0x00] = "ROM_ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1_RAM",
    [0x03] = "MBC1_RAM_BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2_BATTERY",
    [0x08] = "ROM_RAM_1",
    [0x09] = "ROM_RAM_BATTERY_1",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01_RAM",
    [0x0D] = "MMM01_RAM_BATTERY",
    [0x0F] = "MBC3_TIMER_BATTERY",
    [0x10] = "MBC3_TIMER_RAM_BATTERY_2",
    [0x11] = "MBC3",
    [0x12] = "MBC3_RAM_2",
    [0x13] = "MBC3_RAM_BATTERY_2",
    [0x19] = "MBC5",
    [0x1A] = "MBC5_RAM",
    [0x1B] = "MBC5_RAM_BATTERY",
    [0x1C] = "MBC5_RUMBLE",
    [0x1D] = "MBC5_RUMBLE_RAM",
    [0x1E] = "MBC5_RUMBLE_RAM_BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7_SENSOR_RUMBLE_RAM_BATTERY",
    [0xFC] = "POCKET_CAMERA",
    [0xFD] = "BANDAI_TAMA5",
    [0xFE] = "HUC3",
    [0xFF] = "HUC1_RAM_BATTERY",
};

void debug_instructions(uint8_t const rom[], uint16_t const offset, size_t const nb_instructions) {
    uint16_t pc = offset;
    for(size_t i = 0; i < nb_instructions; i++) {
        switch(rom[pc]) {
            case 0x00:
                printf("%04x NOP\n", pc);
                pc++;
                break;
            case 0x01:
                printf("%04x LD BC, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0x02:
                printf("%04x LD (BC), A\n", pc);
                pc++;
                break;
            case 0x03:
                printf("%04x INC BC\n", pc);
                pc++;
                break;
            case 0x04:
                printf("%04x INC B\n", pc);
                pc++;
                break;
            case 0x05:
                printf("%04x DEC B\n", pc);
                pc++;
                break;
            case 0x06:
                printf("%04x LD B,%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x07:
                printf("%04x RLCA\n", pc);
                pc++;
                break;
            case 0x08:
                printf("%04x LD (0x%02x%02x), SP\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0x09:
                printf("%04x ADD HL, BC\n", pc);
                pc++;
                break;
            case 0x0a:
                printf("%04x LD A, (BC)\n", pc);
                pc++;
                break;
            case 0x0b:
                printf("%04x DEC BC\n", pc);
                pc++;
                break;
            case 0x0c:
                printf("%04x INC C\n", pc);
                pc++;
                break;
            case 0x0d:
                printf("%04x DEC C\n", pc);
                pc++;
                break;
            case 0x0e:
                printf("%04x LD C, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x0f:
                printf("%04x RRCA\n", pc);
                pc++;
                break;

            case 0x10:
                printf("%04x STOP\n", pc);
                pc++;
                break;
            case 0x11:
                printf("%04x LD DE, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0x12:
                printf("%04x LD (DE), A\n", pc);
                pc++;
                break;
            case 0x13:
                printf("%04x INC DE\n", pc);
                pc++;
                break;
            case 0x14:
                printf("%04x INC D\n", pc);
                pc++;
                break;
            case 0x15:
                printf("%04x DEC D\n", pc);
                pc++;
                break;
            case 0x16:
                printf("%04x LD D,%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x17:
                printf("%04x RLA\n", pc);
                pc++;
                break;
            case 0x18:
                printf("%04x JR, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x19:
                printf("%04x ADD HL, DE\n", pc);
                pc++;
                break;
            case 0x1a:
                printf("%04x LD A, (DE)\n", pc);
                pc++;
                break;
            case 0x1b:
                printf("%04x DEC DE\n", pc);
                pc++;
                break;
            case 0x1c:
                printf("%04x INC E\n", pc);
                pc++;
                break;
            case 0x1d:
                printf("%04x DEC E\n", pc);
                pc++;
                break;
            case 0x1e:
                printf("%04x LD E, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x1f:
                printf("%04x RRA\n", pc);
                pc++;
                break;

            case 0x20:
                printf("%04x JR NZ, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x21:
                printf("%04x LD HL, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0x22:
                printf("%04x LD (HL+), A\n", pc);
                pc++;
                break;
            case 0x23:
                printf("%04x INC HL\n", pc);
                pc++;
                break;
            case 0x24:
                printf("%04x INC H\n", pc);
                pc++;
                break;
            case 0x25:
                printf("%04x DEC H\n", pc);
                pc++;
                break;
            case 0x26:
                printf("%04x LD H,%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x27:
                printf("%04x DAA\n", pc);
                pc++;
                break;
            case 0x28:
                printf("%04x JR Z, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x29:
                printf("%04x ADD HL, HL\n", pc);
                pc++;
                break;
            case 0x2a:
                printf("%04x LD A, (HL+)\n", pc);
                pc++;
                break;
            case 0x2b:
                printf("%04x DEC HL\n", pc);
                pc++;
                break;
            case 0x2c:
                printf("%04x INC L\n", pc);
                pc++;
                break;
            case 0x2d:
                printf("%04x DEC L\n", pc);
                pc++;
                break;
            case 0x2e:
                printf("%04x LD L, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x2f:
                printf("%04x CPL\n", pc);
                pc++;
                break;

            case 0x30:
                printf("%04x JR NC, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x31:
                printf("%04x LD SP, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0x32:
                printf("%04x LD (HL-), A\n", pc);
                pc++;
                break;
            case 0x33:
                printf("%04x INC SP\n", pc);
                pc++;
                break;
            case 0x34:
                printf("%04x INC (HL)\n", pc);
                pc++;
                break;
            case 0x35:
                printf("%04x DEC (HL)\n", pc);
                pc++;
                break;
            case 0x36:
                printf("%04x LD (HL),%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x37:
                printf("%04x SCF\n", pc);
                pc++;
                break;
            case 0x38:
                printf("%04x JR C, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x39:
                printf("%04x ADD HL, SP\n", pc);
                pc++;
                break;
            case 0x3a:
                printf("%04x LD A, (HL-)\n", pc);
                pc++;
                break;
            case 0x3b:
                printf("%04x DEC SP\n", pc);
                pc++;
                break;
            case 0x3c:
                printf("%04x INC A\n", pc);
                pc++;
                break;
            case 0x3d:
                printf("%04x DEC A\n", pc);
                pc++;
                break;
            case 0x3e:
                printf("%04x LD A, 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0x3f:
                printf("%04x CCF\n", pc);
                pc++;
                break;

            case 0x40:
                printf("%04x LD B,B\n", pc);
                pc++;
                break;
            case 0x41:
                printf("%04x LD B,C\n", pc);
                pc++;
                break;
            case 0x42:
                printf("%04x LD B,D\n", pc);
                pc++;
                break;
            case 0x43:
                printf("%04x LD B,E\n", pc);
                pc++;
                break;
            case 0x44:
                printf("%04x LD B,H\n", pc);
                pc++;
                break;
            case 0x45:
                printf("%04x LD B,L\n", pc);
                pc++;
                break;
            case 0x46:
                printf("%04x LD B,(HL)\n", pc);
                pc++;
                break;
            case 0x47:
                printf("%04x LD B,A\n", pc);
                pc++;
                break;
            case 0x48:
                printf("%04x LD C,B\n", pc);
                pc++;
                break;
            case 0x49:
                printf("%04x LD C,C\n", pc);
                pc++;
                break;
            case 0x4a:
                printf("%04x LD C,D\n", pc);
                pc++;
                break;
            case 0x4b:
                printf("%04x LD C,E\n", pc);
                pc++;
                break;
            case 0x4c:
                printf("%04x LD C,H\n", pc);
                pc++;
                break;
            case 0x4d:
                printf("%04x LD C,L\n", pc);
                pc++;
                break;
            case 0x4e:
                printf("%04x LD C,(HL)\n", pc);
                pc++;
                break;
            case 0x4f:
                printf("%04x LD C,A\n", pc);
                pc++;
                break;

            case 0x50:
                printf("%04x LD D,B\n", pc);
                pc++;
                break;
            case 0x51:
                printf("%04x LD D,C\n", pc);
                pc++;
                break;
            case 0x52:
                printf("%04x LD D,D\n", pc);
                pc++;
                break;
            case 0x53:
                printf("%04x LD D,E\n", pc);
                pc++;
                break;
            case 0x54:
                printf("%04x LD D,H\n", pc);
                pc++;
                break;
            case 0x55:
                printf("%04x LD D,L\n", pc);
                pc++;
                break;
            case 0x56:
                printf("%04x LD D,(HL)\n", pc);
                pc++;
                break;
            case 0x57:
                printf("%04x LD D,A\n", pc);
                pc++;
                break;
            case 0x58:
                printf("%04x LD E,B\n", pc);
                pc++;
                break;
            case 0x59:
                printf("%04x LD E,C\n", pc);
                pc++;
                break;
            case 0x5a:
                printf("%04x LD E,D\n", pc);
                pc++;
                break;
            case 0x5b:
                printf("%04x LD E,E\n", pc);
                pc++;
                break;
            case 0x5c:
                printf("%04x LD E,H\n", pc);
                pc++;
                break;
            case 0x5d:
                printf("%04x LD E,L\n", pc);
                pc++;
                break;
            case 0x5e:
                printf("%04x LD E,(HL)\n", pc);
                pc++;
                break;
            case 0x5f:
                printf("%04x LD E,A\n", pc);
                pc++;
                break;

            case 0x60:
                printf("%04x LD H,B\n", pc);
                pc++;
                break;
            case 0x61:
                printf("%04x LD H,C\n", pc);
                pc++;
                break;
            case 0x62:
                printf("%04x LD H,D\n", pc);
                pc++;
                break;
            case 0x63:
                printf("%04x LD H,E\n", pc);
                pc++;
                break;
            case 0x64:
                printf("%04x LD H,H\n", pc);
                pc++;
                break;
            case 0x65:
                printf("%04x LD H,L\n", pc);
                pc++;
                break;
            case 0x66:
                printf("%04x LD H,(HL)\n", pc);
                pc++;
                break;
            case 0x67:
                printf("%04x LD H,A\n", pc);
                pc++;
                break;
            case 0x68:
                printf("%04x LD E,B\n", pc);
                pc++;
                break;
            case 0x69:
                printf("%04x LD E,C\n", pc);
                pc++;
                break;
            case 0x6a:
                printf("%04x LD E,D\n", pc);
                pc++;
                break;
            case 0x6b:
                printf("%04x LD E,E\n", pc);
                pc++;
                break;
            case 0x6c:
                printf("%04x LD E,H\n", pc);
                pc++;
                break;
            case 0x6d:
                printf("%04x LD E,L\n", pc);
                pc++;
                break;
            case 0x6e:
                printf("%04x LD E,(HL)\n", pc);
                pc++;
                break;
            case 0x6f:
                printf("%04x LD E,A\n", pc);
                pc++;
                break;

            case 0x70:
                printf("%04x LD (HL),B\n", pc);
                pc++;
                break;
            case 0x71:
                printf("%04x LD (HL),C\n", pc);
                pc++;
                break;
            case 0x72:
                printf("%04x LD (HL),D\n", pc);
                pc++;
                break;
            case 0x73:
                printf("%04x LD (HL),E\n", pc);
                pc++;
                break;
            case 0x74:
                printf("%04x LD (HL),H\n", pc);
                pc++;
                break;
            case 0x75:
                printf("%04x LD (HL),L\n", pc);
                pc++;
                break;
            case 0x76:
                printf("%04x HALT\n", pc);
                pc++;
                break;
            case 0x77:
                printf("%04x LD (HL),A\n", pc);
                pc++;
                break;
            case 0x78:
                printf("%04x LD A,B\n", pc);
                pc++;
                break;
            case 0x79:
                printf("%04x LD A,C\n", pc);
                pc++;
                break;
            case 0x7a:
                printf("%04x LD A,D\n", pc);
                pc++;
                break;
            case 0x7b:
                printf("%04x LD A,E\n", pc);
                pc++;
                break;
            case 0x7c:
                printf("%04x LD A,H\n", pc);
                pc++;
                break;
            case 0x7d:
                printf("%04x LD A,L\n", pc);
                pc++;
                break;
            case 0x7e:
                printf("%04x LD A,(HL)\n", pc);
                pc++;
                break;
            case 0x7f:
                printf("%04x LD A,A\n", pc);
                pc++;
                break;

            case 0x80:
                printf("%04x ADD A,B\n", pc);
                pc++;
                break;
            case 0x81:
                printf("%04x ADD A,C\n", pc);
                pc++;
                break;
            case 0x82:
                printf("%04x ADD A,D\n", pc);
                pc++;
                break;
            case 0x83:
                printf("%04x ADD A,E\n", pc);
                pc++;
                break;
            case 0x84:
                printf("%04x ADD A,H\n", pc);
                pc++;
                break;
            case 0x85:
                printf("%04x ADD A,L\n", pc);
                pc++;
                break;
            case 0x86:
                printf("%04x ADD A,(HL)\n", pc);
                pc++;
                break;
            case 0x87:
                printf("%04x ADD A,A\n", pc);
                pc++;
                break;
            case 0x88:
                printf("%04x ADC A,B\n", pc);
                pc++;
                break;
            case 0x89:
                printf("%04x ADC A,C\n", pc);
                pc++;
                break;
            case 0x8a:
                printf("%04x ADC A,D\n", pc);
                pc++;
                break;
            case 0x8b:
                printf("%04x ADC A,E\n", pc);
                pc++;
                break;
            case 0x8c:
                printf("%04x ADC A,H\n", pc);
                pc++;
                break;
            case 0x8d:
                printf("%04x ADC A,L\n", pc);
                pc++;
                break;
            case 0x8e:
                printf("%04x ADC A,(HL)\n", pc);
                pc++;
                break;
            case 0x8f:
                printf("%04x ADC A,A\n", pc);
                pc++;
                break;
            case 0x90:
                printf("%04x SUB B\n", pc);
                pc++;
                break;
            case 0x91:
                printf("%04x SUB C\n", pc);
                pc++;
                break;
            case 0x92:
                printf("%04x SUB D\n", pc);
                pc++;
                break;
            case 0x93:
                printf("%04x SUB E\n", pc);
                pc++;
                break;
            case 0x94:
                printf("%04x SUB H\n", pc);
                pc++;
                break;
            case 0x95:
                printf("%04x SUB L\n", pc);
                pc++;
                break;
            case 0x96:
                printf("%04x SUB (HL)\n", pc);
                pc++;
                break;
            case 0x97:
                printf("%04x SUB A\n", pc);
                pc++;
                break;
            case 0x98:
                printf("%04x SBC B\n", pc);
                pc++;
                break;
            case 0x99:
                printf("%04x SBC C\n", pc);
                pc++;
                break;
            case 0x9a:
                printf("%04x SBC D\n", pc);
                pc++;
                break;
            case 0x9b:
                printf("%04x SBC E\n", pc);
                pc++;
                break;
            case 0x9c:
                printf("%04x SBC H\n", pc);
                pc++;
                break;
            case 0x9d:
                printf("%04x SBC L\n", pc);
                pc++;
                break;
            case 0x9e:
                printf("%04x SBC (HL)\n", pc);
                pc++;
                break;
            case 0x9f:
                printf("%04x SBC A\n", pc);
                pc++;
                break;

            case 0xa0:
                printf("%04x AND B\n", pc);
                pc++;
                break;
            case 0xa1:
                printf("%04x AND C\n", pc);
                pc++;
                break;
            case 0xa2:
                printf("%04x AND D\n", pc);
                pc++;
                break;
            case 0xa3:
                printf("%04x AND E\n", pc);
                pc++;
                break;
            case 0xa4:
                printf("%04x AND H\n", pc);
                pc++;
                break;
            case 0xa5:
                printf("%04x AND L\n", pc);
                pc++;
                break;
            case 0xa6:
                printf("%04x AND (HL)\n", pc);
                pc++;
                break;
            case 0xa7:
                printf("%04x AND A\n", pc);
                pc++;
                break;
            case 0xa8:
                printf("%04x XOR B\n", pc);
                pc++;
                break;
            case 0xa9:
                printf("%04x XOR C\n", pc);
                pc++;
                break;
            case 0xaa:
                printf("%04x XOR D\n", pc);
                pc++;
                break;
            case 0xab:
                printf("%04x XOR E\n", pc);
                pc++;
                break;
            case 0xac:
                printf("%04x XOR H\n", pc);
                pc++;
                break;
            case 0xad:
                printf("%04x XOR L\n", pc);
                pc++;
                break;
            case 0xae:
                printf("%04x XOR (HL)\n", pc);
                pc++;
                break;
            case 0xaf:
                printf("%04x XOR A\n", pc);
                pc++;
                break;

            case 0xb0:
                printf("%04x OR B\n", pc);
                pc++;
                break;
            case 0xb1:
                printf("%04x OR C\n", pc);
                pc++;
                break;
            case 0xb2:
                printf("%04x OR D\n", pc);
                pc++;
                break;
            case 0xb3:
                printf("%04x OR E\n", pc);
                pc++;
                break;
            case 0xb4:
                printf("%04x OR H\n", pc);
                pc++;
                break;
            case 0xb5:
                printf("%04x OR L\n", pc);
                pc++;
                break;
            case 0xb6:
                printf("%04x OR (HL)\n", pc);
                pc++;
                break;
            case 0xb7:
                printf("%04x OR A\n", pc);
                pc++;
                break;
            case 0xb8:
                printf("%04x CP B\n", pc);
                pc++;
                break;
            case 0xb9:
                printf("%04x CP C\n", pc);
                pc++;
                break;
            case 0xba:
                printf("%04x CP D\n", pc);
                pc++;
                break;
            case 0xbb:
                printf("%04x CP E\n", pc);
                pc++;
                break;
            case 0xbc:
                printf("%04x CP H\n", pc);
                pc++;
                break;
            case 0xbd:
                printf("%04x CP L\n", pc);
                pc++;
                break;
            case 0xbe:
                printf("%04x CP (HL)\n", pc);
                pc++;
                break;
            case 0xbf:
                printf("%04x CP A\n", pc);
                pc++;
                break;

            case 0xc0:
                printf("%04x RET NZ\n", pc);
                pc++;
                break;
            case 0xc1:
                printf("%04x POP BC\n", pc);
                pc++;
                break;
            case 0xc2:
                printf("%04x JP NZ, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xc3:
                printf("%04x JP 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xc4:
                printf("%04x CALL NZ, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xc5:
                printf("%04x PUSH BC\n", pc);
                pc++;
                break;
            case 0xc6:
                printf("%04x ADD A,0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xc7:
                printf("%04x RST 0\n", pc);
                pc++;
                break;
            case 0xc8:
                printf("%04x RET Z\n", pc);
                pc++;
                break;
            case 0xc9:
                printf("%04x RET\n", pc);
                pc++;
                break;
            case 0xca:
                printf("%04x JP Z, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            // CB
            case 0xcb:
                pc++;
                debug_cb_instruction(rom, pc);
                pc++;
                break;
            case 0xcc:
                printf("%04x CALL Z, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xcd:
                printf("%04x CALL 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xce:
                printf("%04x ADC A,0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xcf:
                printf("%04x RST 1\n", pc);
                pc++;
                break;

            case 0xd0:
                printf("%04x RET NC\n", pc);
                pc++;
                break;
            case 0xd1:
                printf("%04x POP DE\n", pc);
                pc++;
                break;
            case 0xd2:
                printf("%04x JP NC, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xd4:
                printf("%04x CALL NC, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xd5:
                printf("%04x PUSH DE\n", pc);
                pc++;
                break;
            case 0xd6:
                printf("%04x SUB 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xd7:
                printf("%04x RST 2\n", pc);
                pc++;
                break;
            case 0xd8:
                printf("%04x RET C\n", pc);
                pc++;
                break;
            case 0xd9:
                printf("%04x RETI\n", pc);
                pc++;
                break;
            case 0xda:
                printf("%04x JP C, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xdc:
                printf("%04x CALL C, 0x%02x%02x\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xde:
                printf("%04x SBC 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xdf:
                printf("%04x RST 3\n", pc);
                pc++;
                break;

            case 0xe0:
                printf("%04x LD (0xff%02x), A\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xe1:
                printf("%04x POP HL\n", pc);
                pc++;
                break;
            case 0xe2:
                printf("%04x LD (C),A\n", pc);
                pc++;
                break;
            case 0xe5:
                printf("%04x PUSH HL\n", pc);
                pc++;
                break;
            case 0xe6:
                printf("%04x AND 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xe7:
                printf("%04x RST 4\n", pc);
                pc++;
                break;
            case 0xe8:
                printf("%04x ADD SP, s(0x%02x)\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xe9:
                printf("%04x JP HL\n", pc);
                pc++;
                break;
            case 0xea:
                printf("%04x LD (0x%02x%02x), A\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xee:
                printf("%04x XOR 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xef:
                printf("%04x RST 5\n", pc);
                pc++;
                break;
            case 0xf0:
                printf("%04x LD A, (0xff%02x)\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xf1:
                printf("%04x POP AF\n", pc);
                pc++;
                break;
            case 0xf2:
                printf("%04x LD A, (C)\n", pc);
                pc++;
                break;
            case 0xf3:
                printf("%04x DI\n", pc);
                pc++;
                break;
            case 0xf5:
                printf("%04x PUSH AF\n", pc);
                pc++;
                break;
            case 0xf6:
                printf("%04x OR 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xf7:
                printf("%04x RST 6\n", pc);
                pc++;
                break;
            case 0xf8:
                printf("%04x LD HL, SP+s(0x%02x)\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xf9:
                printf("%04x LD SP, HL\n", pc);
                pc++;
                break;
            case 0xfa:
                printf("%04x LD A, (0x%02x%02x)\n", pc, rom[pc+2], rom[pc+1]);
                pc += 3;
                break;
            case 0xfb:
                printf("%04x EI\n", pc);
                pc++;
                break;
            case 0xfe:
                printf("%04x CP 0x%02x\n", pc, rom[pc+1]);
                pc += 2;
                break;
            case 0xff:
                printf("%04x RST 7\n", pc);
                pc++;
                break;
            default:
                printf("%04x ===== UNKNOWN OPCODE %02x ======\n", pc, rom[pc]);
                pc++;
                break;
        }
    }
}

void debug_cb_instruction(uint8_t const rom[], unsigned int const pc) {
    const char* reg_operand = NULL;
    const char* reg_operator = NULL;
    uint8_t operand = rom[pc] & 0x0f;
    uint8_t operator = (rom[pc] >> 4);
    switch (operand) {
        case 0:
        case 8:
            reg_operand = "B";
            break;
        case 1:
        case 9:
            reg_operand = "C";
            break;
        case 2:
        case 0xa:
            reg_operand = "D";
            break;
        case 3:
        case 0xb:
            reg_operand = "E";
            break;
        case 4:
        case 0xc:
            reg_operand = "H";
            break;
        case 5:
        case 0xd:
            reg_operand = "L";
            break;
        case 6:
        case 0xe:
            reg_operand = "(HL)";
            break;
        case 7:
        case 0xf:
            reg_operand = "A";
            break;
    }
    if (operator == 0 && operand < 0x8) {
        reg_operator = "RLC";
    } else if (operator == 0 && operand >= 0x8) {
        reg_operator = "RRC";
    } else if (operator == 1 && operand < 0x8) {
        reg_operator = "RL";
    } else if (operator == 1 && operand >= 0x8) {
        reg_operator = "RR";
    } else if (operator == 2 && operand < 0x8) {
        reg_operator = "SLA";
    } else if (operator == 2 && operand >= 0x8) {
        reg_operator = "SRA";
    } else if (operator == 3 && operand < 0x8) {
        reg_operator = "SWAP";
    } else if (operator == 3 && operand >= 0x8) {
        reg_operator = "SRL";
    } else if (operator == 4 && operand < 0x8) {
        reg_operator = "BIT 0";
    } else if (operator == 4 && operand >= 0x8) {
        reg_operator = "BIT 1";
    } else if (operator == 5 && operand < 0x8) {
        reg_operator = "BIT 2";
    } else if (operator == 5 && operand >= 0x8) {
        reg_operator = "BIT 3";
    } else if (operator == 6 && operand < 0x8) {
        reg_operator = "BIT 4";
    } else if (operator == 6 && operand >= 0x8) {
        reg_operator = "BIT 5";
    } else if (operator == 7 && operand < 0x8) {
        reg_operator = "BIT 6";
    } else if (operator == 7 && operand >= 0x8) {
        reg_operator = "BIT 7";
    } else if (operator == 8 && operand < 0x8) {
        reg_operator = "RES 0";
    } else if (operator == 8 && operand >= 0x8) {
        reg_operator = "RES 1";
    } else if (operator == 9 && operand < 0x8) {
        reg_operator = "RES 2";
    } else if (operator == 9 && operand >= 0x8) {
        reg_operator = "RES 3";
    } else if (operator == 0xa && operand < 0x8) {
        reg_operator = "RES 4";
    } else if (operator == 0xa && operand >= 0x8) {
        reg_operator = "RES 5";
    } else if (operator == 0xb && operand < 0x8) {
        reg_operator = "RES 6";
    } else if (operator == 0xb && operand >= 0x8) {
        reg_operator = "RES 7";
    } else if (operator == 0xc && operand < 0x8) {
        reg_operator = "SET 0";
    } else if (operator == 0xc && operand >= 0x8) {
        reg_operator = "SET 1";
    } else if (operator == 0xd && operand < 0x8) {
        reg_operator = "SET 2";
    } else if (operator == 0xd && operand >= 0x8) {
        reg_operator = "SET 3";
    } else if (operator == 0xa && operand < 0x8) {
        reg_operator = "SET 4";
    } else if (operator == 0xe && operand >= 0x8) {
        reg_operator = "SET 5";
    } else if (operator == 0xf && operand < 0x8) {
        reg_operator = "SET 6";
    } else if (operator == 0xf && operand >= 0x8) {
        reg_operator = "SET 7";
    }
    printf("%02x %s, %s\n", pc-1, reg_operator, reg_operand);
}

void debug_registers(CpuRegisters* const regs) {
    printf("a = 0x%2x\n", regs->a);
    printf("b = 0x%2x\n", regs->b);
    printf("c = 0x%2x\n", regs->c);
    printf("d = 0x%2x\n", regs->d);
    printf("e = 0x%2x\n", regs->e);
    printf("h = 0x%2x\n", regs->h);
    printf("l = 0x%2x\n", regs->l);
    printf("f = 0x%2x\n", regs->f);

    printf("=======\n");

    printf("af = 0x%4x\n", regs->af);
    printf("bc = 0x%4x\n", regs->bc);
    printf("de = 0x%4x\n", regs->de);
    printf("hl = 0x%4x\n", regs->hl);
    printf("sp = 0x%4x\n", regs->sp);
    printf("pc = 0x%4x\n", regs->pc);

    printf("=======\n");
    printf("FLAGS:\n");
    printf("c, h, n, z\n");
    printf("%1d, %1d, %1d, %1d\n", regs->f_c, regs->f_h, regs->f_n, regs->f_z);
}

void debug_header(RomHeader* const rom_header, uint8_t const rom[static 0x150]) {
    /* putchar('\n'); */
    /* fputs("logo: ", stdout); */
    /* for(int i = 0; i < 0x30; i++) { */
    /*     printf("%x ", rom_header->logo[i]); */
    /* } */
    /* putchar('\n'); */
    fputs("title: ", stdout);
    for(int i = 0; i < 15; i++) {
        if (rom_header->title[i] == 0) {
            break;
        }
        putchar(rom_header->title[i]);
    }
    putchar('\n');
    printf("cgb_flag: 0x%02x\n", rom_header->cgb_flag);
    printf("new_licence_code: 0x%04x\n", rom_header->new_licence_code);
    printf("sgb_flag: 0x%02x\n", rom_header->sgb_flag);
    printf("cartridge_type: %s\n", get_rom_cartridge_type(rom_header));
    printf("rom_size: 0x%02x\n", rom_header->rom_size);
    printf("ram_size: 0x%02x\n", rom_header->ram_size);
    printf("destination_code: 0x%02x\n", rom_header->destination_code);
    printf("old_licence_code: 0x%02x\n", rom_header->old_licence_code);
    printf("mask_rom_version_number: 0x%02x\n", rom_header->mask_rom_version_number);
    printf("header_checksum: 0x%02x\n", rom_header->header_checksum);
    printf("global_checksum: 0x%04x\n", rom_header->global_checksum);
    printf("is header checksum valid ? : %s\n", is_header_checksum_valid(rom_header, rom) ? "true" : "false");
}

const char* get_rom_cartridge_type(RomHeader* const rom_header) {
    const char* rom_type = CartridgeType[rom_header->cartridge_type];
    if (rom_type[0] == '\0') {
        return "UNKNOWN CARTRIDGE TYPE";
    }
    return rom_type;
}

