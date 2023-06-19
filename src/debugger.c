#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cpu.h"
#include "mmu.h"
#include "debugger.h"
#include "cartridge.h"
#include "instruction.h"

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

void debug_instruction(char* buffer, uint8_t const rom[], uint16_t const pc) {
    FILE* buffer_stream = fmemopen(buffer, DEBUG_INSTRUCTION_BUFFER_SIZE, "w");
    switch(rom[pc]) {
        case 0x00:
            fprintf(buffer_stream, "NOP");
            break;
        case 0x01:
            fprintf(buffer_stream, "LD BC, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0x02:
            fprintf(buffer_stream, "LD (BC), A");
            break;
        case 0x03:
            fprintf(buffer_stream, "INC BC");
            break;
        case 0x04:
            fprintf(buffer_stream, "INC B");
            break;
        case 0x05:
            fprintf(buffer_stream, "DEC B");
            break;
        case 0x06:
            fprintf(buffer_stream, "LD B,%02x", rom[pc+1]);
            break;
        case 0x07:
            fprintf(buffer_stream, "RLCA");
            break;
        case 0x08:
            fprintf(buffer_stream, "LD (0x%02x%02x), SP", rom[pc+2], rom[pc+1]);
            break;
        case 0x09:
            fprintf(buffer_stream, "ADD HL, BC");
            break;
        case 0x0a:
            fprintf(buffer_stream, "LD A, (BC)");
            break;
        case 0x0b:
            fprintf(buffer_stream, "DEC BC");
            break;
        case 0x0c:
            fprintf(buffer_stream, "INC C");
            break;
        case 0x0d:
            fprintf(buffer_stream, "DEC C");
            break;
        case 0x0e:
            fprintf(buffer_stream, "LD C, 0x%02x", rom[pc+1]);
            break;
        case 0x0f:
            fprintf(buffer_stream, "RRCA");
            break;
        case 0x10:
            fprintf(buffer_stream, "STOP");
            break;
        case 0x11:
            fprintf(buffer_stream, "LD DE, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0x12:
            fprintf(buffer_stream, "LD (DE), A");
            break;
        case 0x13:
            fprintf(buffer_stream, "INC DE");
            break;
        case 0x14:
            fprintf(buffer_stream, "INC D");
            break;
        case 0x15:
            fprintf(buffer_stream, "DEC D");
            break;
        case 0x16:
            fprintf(buffer_stream, "LD D,%02x", rom[pc+1]);
            break;
        case 0x17:
            fprintf(buffer_stream, "RLA");
            break;
        case 0x18:
            fprintf(buffer_stream, "JR, 0x%02x", rom[pc+1]);
            break;
        case 0x19:
            fprintf(buffer_stream, "ADD HL, DE");
            break;
        case 0x1a:
            fprintf(buffer_stream, "LD A, (DE)");
            break;
        case 0x1b:
            fprintf(buffer_stream, "DEC DE");
            break;
        case 0x1c:
            fprintf(buffer_stream, "INC E");
            break;
        case 0x1d:
            fprintf(buffer_stream, "DEC E");
            break;
        case 0x1e:
            fprintf(buffer_stream, "LD E, 0x%02x", rom[pc+1]);
            break;
        case 0x1f:
            fprintf(buffer_stream, "RRA");
            break;
        case 0x20:
            fprintf(buffer_stream, "JR NZ, 0x%02x", rom[pc+1]);
            break;
        case 0x21:
            fprintf(buffer_stream, "LD HL, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0x22:
            fprintf(buffer_stream, "LD (HL+), A");
            break;
        case 0x23:
            fprintf(buffer_stream, "INC HL");
            break;
        case 0x24:
            fprintf(buffer_stream, "INC H");
            break;
        case 0x25:
            fprintf(buffer_stream, "DEC H");
            break;
        case 0x26:
            fprintf(buffer_stream, "LD H,%02x", rom[pc+1]);
            break;
        case 0x27:
            fprintf(buffer_stream, "DAA");
            break;
        case 0x28:
            fprintf(buffer_stream, "JR Z, 0x%02x", rom[pc+1]);
            break;
        case 0x29:
            fprintf(buffer_stream, "ADD HL, HL");
            break;
        case 0x2a:
            fprintf(buffer_stream, "LD A, (HL+)");
            break;
        case 0x2b:
            fprintf(buffer_stream, "DEC HL");
            break;
        case 0x2c:
            fprintf(buffer_stream, "INC L");
            break;
        case 0x2d:
            fprintf(buffer_stream, "DEC L");
            break;
        case 0x2e:
            fprintf(buffer_stream, "LD L, 0x%02x", rom[pc+1]);
            break;
        case 0x2f:
            fprintf(buffer_stream, "CPL");
            break;
        case 0x30:
            fprintf(buffer_stream, "JR NC, 0x%02x", rom[pc+1]);
            break;
        case 0x31:
            fprintf(buffer_stream, "LD SP, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0x32:
            fprintf(buffer_stream, "LD (HL-), A");
            break;
        case 0x33:
            fprintf(buffer_stream, "INC SP");
            break;
        case 0x34:
            fprintf(buffer_stream, "INC (HL)");
            break;
        case 0x35:
            fprintf(buffer_stream, "DEC (HL)");
            break;
        case 0x36:
            fprintf(buffer_stream, "LD (HL),%02x", rom[pc+1]);
            break;
        case 0x37:
            fprintf(buffer_stream, "SCF");
            break;
        case 0x38:
            fprintf(buffer_stream, "JR C, 0x%02x", rom[pc+1]);
            break;
        case 0x39:
            fprintf(buffer_stream, "ADD HL, SP");
            break;
        case 0x3a:
            fprintf(buffer_stream, "LD A, (HL-)");
            break;
        case 0x3b:
            fprintf(buffer_stream, "DEC SP");
            break;
        case 0x3c:
            fprintf(buffer_stream, "INC A");
            break;
        case 0x3d:
            fprintf(buffer_stream, "DEC A");
            break;
        case 0x3e:
            fprintf(buffer_stream, "LD A, 0x%02x", rom[pc+1]);
            break;
        case 0x3f:
            fprintf(buffer_stream, "CCF");
            break;
        case 0x40:
            fprintf(buffer_stream, "LD B,B");
            break;
        case 0x41:
            fprintf(buffer_stream, "LD B,C");
            break;
        case 0x42:
            fprintf(buffer_stream, "LD B,D");
            break;
        case 0x43:
            fprintf(buffer_stream, "LD B,E");
            break;
        case 0x44:
            fprintf(buffer_stream, "LD B,H");
            break;
        case 0x45:
            fprintf(buffer_stream, "LD B,L");
            break;
        case 0x46:
            fprintf(buffer_stream, "LD B,(HL)");
            break;
        case 0x47:
            fprintf(buffer_stream, "LD B,A");
            break;
        case 0x48:
            fprintf(buffer_stream, "LD C,B");
            break;
        case 0x49:
            fprintf(buffer_stream, "LD C,C");
            break;
        case 0x4a:
            fprintf(buffer_stream, "LD C,D");
            break;
        case 0x4b:
            fprintf(buffer_stream, "LD C,E");
            break;
        case 0x4c:
            fprintf(buffer_stream, "LD C,H");
            break;
        case 0x4d:
            fprintf(buffer_stream, "LD C,L");
            break;
        case 0x4e:
            fprintf(buffer_stream, "LD C,(HL)");
            break;
        case 0x4f:
            fprintf(buffer_stream, "LD C,A");
            break;
        case 0x50:
            fprintf(buffer_stream, "LD D,B");
            break;
        case 0x51:
            fprintf(buffer_stream, "LD D,C");
            break;
        case 0x52:
            fprintf(buffer_stream, "LD D,D");
            break;
        case 0x53:
            fprintf(buffer_stream, "LD D,E");
            break;
        case 0x54:
            fprintf(buffer_stream, "LD D,H");
            break;
        case 0x55:
            fprintf(buffer_stream, "LD D,L");
            break;
        case 0x56:
            fprintf(buffer_stream, "LD D,(HL)");
            break;
        case 0x57:
            fprintf(buffer_stream, "LD D,A");
            break;
        case 0x58:
            fprintf(buffer_stream, "LD E,B");
            break;
        case 0x59:
            fprintf(buffer_stream, "LD E,C");
            break;
        case 0x5a:
            fprintf(buffer_stream, "LD E,D");
            break;
        case 0x5b:
            fprintf(buffer_stream, "LD E,E");
            break;
        case 0x5c:
            fprintf(buffer_stream, "LD E,H");
            break;
        case 0x5d:
            fprintf(buffer_stream, "LD E,L");
            break;
        case 0x5e:
            fprintf(buffer_stream, "LD E,(HL)");
            break;
        case 0x5f:
            fprintf(buffer_stream, "LD E,A");
            break;
        case 0x60:
            fprintf(buffer_stream, "LD H,B");
            break;
        case 0x61:
            fprintf(buffer_stream, "LD H,C");
            break;
        case 0x62:
            fprintf(buffer_stream, "LD H,D");
            break;
        case 0x63:
            fprintf(buffer_stream, "LD H,E");
            break;
        case 0x64:
            fprintf(buffer_stream, "LD H,H");
            break;
        case 0x65:
            fprintf(buffer_stream, "LD H,L");
            break;
        case 0x66:
            fprintf(buffer_stream, "LD H,(HL)");
            break;
        case 0x67:
            fprintf(buffer_stream, "LD H,A");
            break;
        case 0x68:
            fprintf(buffer_stream, "LD E,B");
            break;
        case 0x69:
            fprintf(buffer_stream, "LD E,C");
            break;
        case 0x6a:
            fprintf(buffer_stream, "LD E,D");
            break;
        case 0x6b:
            fprintf(buffer_stream, "LD E,E");
            break;
        case 0x6c:
            fprintf(buffer_stream, "LD E,H");
            break;
        case 0x6d:
            fprintf(buffer_stream, "LD E,L");
            break;
        case 0x6e:
            fprintf(buffer_stream, "LD E,(HL)");
            break;
        case 0x6f:
            fprintf(buffer_stream, "LD E,A");
            break;
        case 0x70:
            fprintf(buffer_stream, "LD (HL),B");
            break;
        case 0x71:
            fprintf(buffer_stream, "LD (HL),C");
            break;
        case 0x72:
            fprintf(buffer_stream, "LD (HL),D");
            break;
        case 0x73:
            fprintf(buffer_stream, "LD (HL),E");
            break;
        case 0x74:
            fprintf(buffer_stream, "LD (HL),H");
            break;
        case 0x75:
            fprintf(buffer_stream, "LD (HL),L");
            break;
        case 0x76:
            fprintf(buffer_stream, "HALT");
            break;
        case 0x77:
            fprintf(buffer_stream, "LD (HL),A");
            break;
        case 0x78:
            fprintf(buffer_stream, "LD A,B");
            break;
        case 0x79:
            fprintf(buffer_stream, "LD A,C");
            break;
        case 0x7a:
            fprintf(buffer_stream, "LD A,D");
            break;
        case 0x7b:
            fprintf(buffer_stream, "LD A,E");
            break;
        case 0x7c:
            fprintf(buffer_stream, "LD A,H");
            break;
        case 0x7d:
            fprintf(buffer_stream, "LD A,L");
            break;
        case 0x7e:
            fprintf(buffer_stream, "LD A,(HL)");
            break;
        case 0x7f:
            fprintf(buffer_stream, "LD A,A");
            break;
        case 0x80:
            fprintf(buffer_stream, "ADD A,B");
            break;
        case 0x81:
            fprintf(buffer_stream, "ADD A,C");
            break;
        case 0x82:
            fprintf(buffer_stream, "ADD A,D");
            break;
        case 0x83:
            fprintf(buffer_stream, "ADD A,E");
            break;
        case 0x84:
            fprintf(buffer_stream, "ADD A,H");
            break;
        case 0x85:
            fprintf(buffer_stream, "ADD A,L");
            break;
        case 0x86:
            fprintf(buffer_stream, "ADD A,(HL)");
            break;
        case 0x87:
            fprintf(buffer_stream, "ADD A,A");
            break;
        case 0x88:
            fprintf(buffer_stream, "ADC A,B");
            break;
        case 0x89:
            fprintf(buffer_stream, "ADC A,C");
            break;
        case 0x8a:
            fprintf(buffer_stream, "ADC A,D");
            break;
        case 0x8b:
            fprintf(buffer_stream, "ADC A,E");
            break;
        case 0x8c:
            fprintf(buffer_stream, "ADC A,H");
            break;
        case 0x8d:
            fprintf(buffer_stream, "ADC A,L");
            break;
        case 0x8e:
            fprintf(buffer_stream, "ADC A,(HL)");
            break;
        case 0x8f:
            fprintf(buffer_stream, "ADC A,A");
            break;
        case 0x90:
            fprintf(buffer_stream, "SUB B");
            break;
        case 0x91:
            fprintf(buffer_stream, "SUB C");
            break;
        case 0x92:
            fprintf(buffer_stream, "SUB D");
            break;
        case 0x93:
            fprintf(buffer_stream, "SUB E");
            break;
        case 0x94:
            fprintf(buffer_stream, "SUB H");
            break;
        case 0x95:
            fprintf(buffer_stream, "SUB L");
            break;
        case 0x96:
            fprintf(buffer_stream, "SUB (HL)");
            break;
        case 0x97:
            fprintf(buffer_stream, "SUB A");
            break;
        case 0x98:
            fprintf(buffer_stream, "SBC B");
            break;
        case 0x99:
            fprintf(buffer_stream, "SBC C");
            break;
        case 0x9a:
            fprintf(buffer_stream, "SBC D");
            break;
        case 0x9b:
            fprintf(buffer_stream, "SBC E");
            break;
        case 0x9c:
            fprintf(buffer_stream, "SBC H");
            break;
        case 0x9d:
            fprintf(buffer_stream, "SBC L");
            break;
        case 0x9e:
            fprintf(buffer_stream, "SBC (HL)");
            break;
        case 0x9f:
            fprintf(buffer_stream, "SBC A");
            break;
        case 0xa0:
            fprintf(buffer_stream, "AND B");
            break;
        case 0xa1:
            fprintf(buffer_stream, "AND C");
            break;
        case 0xa2:
            fprintf(buffer_stream, "AND D");
            break;
        case 0xa3:
            fprintf(buffer_stream, "AND E");
            break;
        case 0xa4:
            fprintf(buffer_stream, "AND H");
            break;
        case 0xa5:
            fprintf(buffer_stream, "AND L");
            break;
        case 0xa6:
            fprintf(buffer_stream, "AND (HL)");
            break;
        case 0xa7:
            fprintf(buffer_stream, "AND A");
            break;
        case 0xa8:
            fprintf(buffer_stream, "XOR B");
            break;
        case 0xa9:
            fprintf(buffer_stream, "XOR C");
            break;
        case 0xaa:
            fprintf(buffer_stream, "XOR D");
            break;
        case 0xab:
            fprintf(buffer_stream, "XOR E");
            break;
        case 0xac:
            fprintf(buffer_stream, "XOR H");
            break;
        case 0xad:
            fprintf(buffer_stream, "XOR L");
            break;
        case 0xae:
            fprintf(buffer_stream, "XOR (HL)");
            break;
        case 0xaf:
            fprintf(buffer_stream, "XOR A");
            break;
        case 0xb0:
            fprintf(buffer_stream, "OR B");
            break;
        case 0xb1:
            fprintf(buffer_stream, "OR C");
            break;
        case 0xb2:
            fprintf(buffer_stream, "OR D");
            break;
        case 0xb3:
            fprintf(buffer_stream, "OR E");
            break;
        case 0xb4:
            fprintf(buffer_stream, "OR H");
            break;
        case 0xb5:
            fprintf(buffer_stream, "OR L");
            break;
        case 0xb6:
            fprintf(buffer_stream, "OR (HL)");
            break;
        case 0xb7:
            fprintf(buffer_stream, "OR A");
            break;
        case 0xb8:
            fprintf(buffer_stream, "CP B");
            break;
        case 0xb9:
            fprintf(buffer_stream, "CP C");
            break;
        case 0xba:
            fprintf(buffer_stream, "CP D");
            break;
        case 0xbb:
            fprintf(buffer_stream, "CP E");
            break;
        case 0xbc:
            fprintf(buffer_stream, "CP H");
            break;
        case 0xbd:
            fprintf(buffer_stream, "CP L");
            break;
        case 0xbe:
            fprintf(buffer_stream, "CP (HL)");
            break;
        case 0xbf:
            fprintf(buffer_stream, "CP A");
            break;
        case 0xc0:
            fprintf(buffer_stream, "RET NZ");
            break;
        case 0xc1:
            fprintf(buffer_stream, "POP BC");
            break;
        case 0xc2:
            fprintf(buffer_stream, "JP NZ, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xc3:
            fprintf(buffer_stream, "JP 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xc4:
            fprintf(buffer_stream, "CALL NZ, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xc5:
            fprintf(buffer_stream, "PUSH BC");
            break;
        case 0xc6:
            fprintf(buffer_stream, "ADD A,0x%02x", rom[pc+1]);
            break;
        case 0xc7:
            fprintf(buffer_stream, "RST 0");
            break;
        case 0xc8:
            fprintf(buffer_stream, "RET Z");
            break;
        case 0xc9:
            fprintf(buffer_stream, "RET");
            break;
        case 0xca:
            fprintf(buffer_stream, "JP Z, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        // CB
        case 0xcb:
            debug_cb_instruction(buffer_stream, rom, pc);
            break;
        case 0xcc:
            fprintf(buffer_stream, "CALL Z, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xcd:
            fprintf(buffer_stream, "CALL 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xce:
            fprintf(buffer_stream, "ADC A,0x%02x", rom[pc+1]);
            break;
        case 0xcf:
            fprintf(buffer_stream, "RST 1");
            break;
        case 0xd0:
            fprintf(buffer_stream, "RET NC");
            break;
        case 0xd1:
            fprintf(buffer_stream, "POP DE");
            break;
        case 0xd2:
            fprintf(buffer_stream, "JP NC, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xd4:
            fprintf(buffer_stream, "CALL NC, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xd5:
            fprintf(buffer_stream, "PUSH DE");
            break;
        case 0xd6:
            fprintf(buffer_stream, "SUB 0x%02x", rom[pc+1]);
            break;
        case 0xd7:
            fprintf(buffer_stream, "RST 2");
            break;
        case 0xd8:
            fprintf(buffer_stream, "RET C");
            break;
        case 0xd9:
            fprintf(buffer_stream, "RETI");
            break;
        case 0xda:
            fprintf(buffer_stream, "JP C, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xdc:
            fprintf(buffer_stream, "CALL C, 0x%02x%02x", rom[pc+2], rom[pc+1]);
            break;
        case 0xde:
            fprintf(buffer_stream, "SBC 0x%02x", rom[pc+1]);
            break;
        case 0xdf:
            fprintf(buffer_stream, "RST 3");
            break;
        case 0xe0:
            fprintf(buffer_stream, "LD (0xff%02x), A", rom[pc+1]);
            break;
        case 0xe1:
            fprintf(buffer_stream, "POP HL");
            break;
        case 0xe2:
            fprintf(buffer_stream, "LD (C),A");
            break;
        case 0xe5:
            fprintf(buffer_stream, "PUSH HL");
            break;
        case 0xe6:
            fprintf(buffer_stream, "AND 0x%02x", rom[pc+1]);
            break;
        case 0xe7:
            fprintf(buffer_stream, "RST 4");
            break;
        case 0xe8:
            fprintf(buffer_stream, "ADD SP, s(0x%02x)", rom[pc+1]);
            break;
        case 0xe9:
            fprintf(buffer_stream, "JP HL");
            break;
        case 0xea:
            fprintf(buffer_stream, "LD (0x%02x%02x), A", rom[pc+2], rom[pc+1]);
            break;
        case 0xee:
            fprintf(buffer_stream, "XOR 0x%02x", rom[pc+1]);
            break;
        case 0xef:
            fprintf(buffer_stream, "RST 5");
            break;
        case 0xf0:
            fprintf(buffer_stream, "LD A, (0xff%02x)", rom[pc+1]);
            break;
        case 0xf1:
            fprintf(buffer_stream, "POP AF");
            break;
        case 0xf2:
            fprintf(buffer_stream, "LD A, (C)");
            break;
        case 0xf3:
            fprintf(buffer_stream, "DI");
            break;
        case 0xf5:
            fprintf(buffer_stream, "PUSH AF");
            break;
        case 0xf6:
            fprintf(buffer_stream, "OR 0x%02x", rom[pc+1]);
            break;
        case 0xf7:
            fprintf(buffer_stream, "RST 6");
            break;
        case 0xf8:
            fprintf(buffer_stream, "LD HL, SP+s(0x%02x)", rom[pc+1]);
            break;
        case 0xf9:
            fprintf(buffer_stream, "LD SP, HL");
            break;
        case 0xfa:
            fprintf(buffer_stream, "LD A, (0x%02x%02x)", rom[pc+2], rom[pc+1]);
            break;
        case 0xfb:
            fprintf(buffer_stream, "EI");
            break;
        case 0xfe:
            fprintf(buffer_stream, "CP 0x%02x", rom[pc+1]);
            break;
        case 0xff:
            fprintf(buffer_stream, "RST 7");
            break;
        default:
            fprintf(buffer_stream, "===== UNKNOWN OPCODE %02x ======", rom[pc]);
            break;
    }
    fclose(buffer_stream);
}

void debug_cb_instruction(FILE* stream, uint8_t const rom[], unsigned int const pc) {
    const char* reg_operand = NULL;
    const char* reg_operator = NULL;
    uint8_t operand = rom[pc+1] & 0x0f;
    uint8_t operator = (rom[pc+1] >> 4);
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
    fprintf(stream, "%02x %s, %s\n", pc-1, reg_operator, reg_operand);
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

void debug_registers_inline(CpuRegisters* const regs) {
    printf("af = 0x%04x ", regs->af);
    printf("bc = 0x%04x ", regs->bc);
    printf("de = 0x%04x ", regs->de);
    printf("hl = 0x%04x ", regs->hl);
    printf("sp = 0x%04x ", regs->sp);
}

void debug_header(RomHeader* const rom_header, uint8_t const rom[static 0x150]) {
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

void debug_flag_inline(CpuRegisters* const regs) {
    if (regs->f_z) {
        printf("Z");
    } else {
        printf("_");
    }
    if (regs->f_n) {
        printf("N");
    } else {
        printf("_");
    }
    if (regs->f_h) {
        printf("H");
    } else {
        printf("_");
    }
    if (regs->f_c) {
        printf("C");
    } else {
        printf("_");
    }
}

static char serial_msg_buffer[0xff] = {0};
static uint16_t serial_msg_length = 0;

static void fetch_character_from_serial_port(Cpu* const cpu) {
    if(mmu_read(cpu, 0xff02) == 0x81) {
        char c = mmu_read(cpu, 0xff01);
        serial_msg_buffer[serial_msg_length] = c;
        if (serial_msg_length < 0x1024) {
            serial_msg_length++;
        } else {
            serial_msg_length = 0;
        }
        mmu_write(cpu, 0xff02, 0);
    }
}

void debug_serial(Cpu* const cpu) {
    fetch_character_from_serial_port(cpu);
    if (serial_msg_buffer[0] != '\0') {
        printf("\nSERIAL_VALUE = %s\n", serial_msg_buffer);
    }
}

char get_debug_serial_buffer(uint16_t index) {
    return serial_msg_buffer[index];
}
