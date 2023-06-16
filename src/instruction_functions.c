#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "instruction_functions.h"

bool check_condition(Cpu* cpu) {
    ConditionFlag condition = cpu->current_instruction->condition;
    uint8_t f_z = cpu->regs.f_z;
    uint8_t f_c = cpu->regs.f_c;
    if (condition == CONDITION_FLAG_NONE) {
        return true;
    } else if (condition == CONDITION_FLAG_Z && f_z == 1) {
        return true;
    } else if (condition == CONDITION_FLAG_NZ && f_z == 0) {
        return true;
    } else if (condition == CONDITION_FLAG_C && f_c == 1) {
        return true;
    } else if (condition == CONDITION_FLAG_NC && f_c == 0) {
        return true;
    }
    return false;
}

void invalid_instruction(Cpu* cpu) {
    fprintf(stderr, "INVALID INSTRUCTION %x\n", cpu->current_opcode);
    exit(1);
}

void ld_instruction(Cpu* cpu) {
    if (cpu->current_destination_in_memory) {
        if (cpu->current_instruction->reg_b >= REGISTER_NAME_AF) {
            cpu->cycles++;
            mmu_write16(cpu, cpu->current_memory_destination, cpu->current_data);
        } else {
            mmu_write(cpu, cpu->current_memory_destination, cpu->current_data);
        }
        return;
    }
    if (cpu->current_instruction->mode == ADDRESSING_MODE_HL_SPR) {
        uint8_t b = get_reg(cpu, cpu->current_instruction->reg_b);
        uint8_t a = cpu->current_data;
        cpu->regs.f_h = (b & 0xf) + (a & 0xf) >= 0x10;
        cpu->regs.f_c = (b & 0xff) + (a & 0xff) >= 0x100;
        set_reg(cpu,
                cpu->current_instruction->reg_a,
                cpu->current_instruction->reg_b + cpu->current_data);
        return;
    }
    // general case
    set_reg(cpu, cpu->current_instruction->reg_a, cpu->current_data);
}

void ldh_instruction(Cpu* cpu) {
    if (cpu->current_instruction->reg_b == REGISTER_NAME_A) {
        set_reg(
            cpu,
            REGISTER_NAME_A,
            mmu_read(cpu,
            0xff00 | cpu->current_data)
        );
    } else {
        mmu_write(cpu, 0xff00 | cpu->current_data, cpu->regs.a);
    }
    cpu->cycles++;
}

void goto_addr(Cpu* cpu, uint16_t addr,  bool pushpc) {
    if (check_condition(cpu)) {
        if (pushpc) {
            stack_push16(cpu, cpu->regs.pc);
            cpu->cycles += 2;
        }
        cpu->regs.pc = addr;
        cpu->cycles += 1;
    }
}

void call_instruction(Cpu* cpu) {
    goto_addr(cpu, cpu->current_data, true);
}

void rst_instruction(Cpu* cpu) {
    goto_addr(cpu, cpu->current_instruction->param, true);
}

void jp_instruction(Cpu* cpu) {
    goto_addr(cpu, cpu->current_data, false);
}

void jr_instruction(Cpu* cpu) {
    int8_t relative_addr = (int8_t)cpu->current_data & 0xff; // may be relative
    uint16_t addr = cpu->regs.pc + relative_addr;
    goto_addr(cpu, addr, false);
}

void nop_instruction(void) {}

void di_instruction(Cpu* cpu) {
    cpu->interruption_master_enable = false;
}

void di_instruction(Cpu* cpu) {
    cpu->enabling_ime = true;
}

void pop_instruction(Cpu* cpu) {
    uint16_t lo = stack_pop(cpu);
    cpu->cycles++;
    uint16_t hi = stack_pop(cpu);
    cpu->cycles++;
    uint16_t value = (hi << 8) | lo;
    set_reg(cpu, cpu->current_instruction->reg_a, value);
    if (cpu->current_instruction->reg_a == REGISTER_NAME_AF) {
        set_reg(cpu, cpu->current_instruction->reg_a, value & 0xfff0);
    }
}

void push_instruction(Cpu* cpu) {
    uint16_t hi = (get_reg(cpu, cpu->current_instruction->reg_a) >> 8) & 0xff;
    cpu->cycles++;
    stack_push(cpu, hi);
    uint16_t lo = get_reg(cpu, cpu->current_instruction->reg_a) & 0xff;
    cpu->cycles++;
    stack_push(cpu, lo);
    cpu->cycles++;
}

void ret_instruction(Cpu* cpu) {
    if (cpu->current_instruction->condition != CONDITION_FLAG_NONE) {
        cpu->cycles++;
    }
    if (check_condition(cpu)) {
        uint16_t hi = stack_pop(cpu);
        cpu->cycles++;
        uint16_t lo = stack_pop(cpu);
        cpu->cycles++;
        cpu->regs.pc = (hi << 8) | lo;
        cpu->cycles++;
    }
}

void reti_instruction(Cpu* cpu) {
    cpu->interruption_master_enable = true;
    ret_instruction(cpu);
}

void inc_instruction(Cpu* cpu) {
    uint16_t value = get_reg(cpu, cpu->current_instruction->reg_a) + 1;
    if (cpu->current_instruction->reg_a >= REGISTER_NAME_AF) {
        cpu->cycles++;
        set_reg(cpu, cpu->current_instruction->reg_a, value);
    }
    if ((cpu->current_instruction->reg_a == REGISTER_NAME_HL) &&
            (cpu->current_instruction->mode == ADDRESSING_MODE_MR)) {
        value = mmu_read(cpu, get_reg(cpu, REGISTER_NAME_HL)) + 1;
        value &= 0xff;
        mmu_write(cpu, get_reg(cpu, REGISTER_NAME_HL), value);
    } else {
        set_reg(cpu, cpu->current_instruction->reg_a, value);
        value = get_reg(cpu, cpu->current_instruction->reg_a);
    }
    if ((cpu->current_opcode & 0x03) == 0x03) {
        return;
    }

    cpu->regs.f_z = value == 0;
    cpu->regs.f_n = 0;
    cpu->regs.f_h = (value & 0x0f) == 0;
}

void dec_instruction(Cpu* cpu) {
    uint16_t value = get_reg(cpu, cpu->current_instruction->reg_a) - 1;
    if (cpu->current_instruction->reg_a >= REGISTER_NAME_AF) {
        cpu->cycles++;
        set_reg(cpu, cpu->current_instruction->reg_a, value);
    }
    if ((cpu->current_instruction->reg_a == REGISTER_NAME_HL) &&
            (cpu->current_instruction->mode = ADDRESSING_MODE_MR)) {
        value = mmu_read(cpu, get_reg(cpu, REGISTER_NAME_HL)) - 1;
        mmu_write(cpu, get_reg(cpu, REGISTER_NAME_HL), value);
    } else {
        set_reg(cpu, cpu->current_instruction->reg_a, value);
        value = get_reg(cpu, cpu->current_instruction->reg_a);
    }
    if ((cpu->current_opcode & 0x0B) == 0x0B) {
        return;
    }

    cpu->regs.f_z = value == 0;
    cpu->regs.f_n = 1;
    cpu->regs.f_h = (value & 0x0f) == 0x0F;
}

void add_instruction(Cpu* cpu) {
    uint32_t value = get_reg(cpu, cpu->current_instruction->reg_a) + cpu->current_data;
    if(cpu->current_instruction->reg_a >= REGISTER_NAME_SP) {
        cpu->cycles++;
        value = get_reg(cpu, REGISTER_NAME_SP) + (int8_t)cpu->current_data;
        cpu->regs.f_z = 0;
        cpu->regs.f_h = (get_reg(cpu, cpu->current_instruction->reg_a) & 0xf) + (cpu->current_data & 0xf) >= 0x10;
        cpu->regs.f_c = (get_reg(cpu, cpu->current_instruction->reg_a) & 0xff) + (cpu->current_data & 0xff) >= 0x100;
    } else if(cpu->current_instruction->reg_a >= REGISTER_NAME_AF) {
        cpu->cycles++;
        cpu->regs.f_z = (value & 0xff) == 0;
        cpu->regs.f_h = (get_reg(cpu, cpu->current_instruction->reg_a) & 0xfff) + (cpu->current_data & 0xfff) >= 0x1000;
        cpu->regs.f_c = (uint32_t)(get_reg(cpu, cpu->current_instruction->reg_a)) + (uint32_t)(cpu->current_data) >= 0x10000;
    } else {
        cpu->regs.f_z = (value & 0xff) == 0;
        cpu->regs.f_h = (get_reg(cpu, cpu->current_instruction->reg_a) & 0xf) + (cpu->current_data & 0xf) >= 0x10;
        cpu->regs.f_c = (get_reg(cpu, cpu->current_instruction->reg_a) & 0xff) + (cpu->current_data & 0xff) >= 0x100;
    }
    set_reg(cpu, cpu->current_instruction->reg_a, value & 0xffff);
}

void adc_instruction(Cpu* cpu) {
    uint16_t u = cpu->current_data;
    uint16_t a = cpu->regs.a;
    uint16_t c = cpu->regs.f_c;

    cpu->regs.a = (u + a + c) & 0xff;

    cpu->regs.f_z = cpu->regs.a == 0;
    cpu->regs.f_n = 0;
    cpu->regs.f_h = (a & 0xf) + (u & 0xf) + c > 0xf;
    cpu->regs.f_c = (a + u + c) > 0xff;

}

void sub_instruction(Cpu* cpu) {
    uint16_t value = cpu->current_data;

    cpu->regs.f_z = cpu->regs.a - value == 0;
    cpu->regs.f_h = (((int)cpu->regs.a & 0xf)
            - (int)(cpu->current_data & 0xf)) < 0;
    cpu->regs.f_c = ((int)cpu->regs.a - (int)(cpu->current_data)) < 0;
    cpu->regs.f_n = 1;

    cpu->regs.a -= value;
}

void sbc_instruction(Cpu* cpu) {
    uint16_t value = get_reg(cpu, cpu->current_instruction->reg_a)
        - cpu->current_data
        - cpu->regs.f_c;

    cpu->regs.f_z = value == 0;
    cpu->regs.f_h = (((int)get_reg(cpu, cpu->current_instruction->reg_a) & 0xf)
            - (int)(cpu->current_data & 0xf))
            - cpu->regs.f_c < 0;
    cpu->regs.f_c = (((int)get_reg(cpu, cpu->current_instruction->reg_a))
            - (int)(cpu->current_data))
            - cpu->regs.f_c < 0;
    cpu->regs.f_n = 1;

    set_reg(cpu, cpu->current_instruction->reg_a, value);
}

void and_instruction(Cpu* cpu) {
    cpu->regs.a &= cpu->current_data;
    cpu->regs.f_z = cpu->regs.a == 0;
    cpu->regs.f_n = 0;
    cpu->regs.f_h = 1;
    cpu->regs.f_c = 0;
}

void xor_instruction(Cpu* cpu) {
    cpu->regs.a ^= cpu->current_data;
    cpu->regs.f = 0;
    cpu->regs.f_z = cpu->regs.a == 0;
}

void or_instruction(Cpu* cpu) {
    cpu->regs.a |= cpu->current_data;
    cpu->regs.f = 0;
    cpu->regs.f_z = cpu->regs.a == 0;
}

void cp_instruction(Cpu* cpu) {
    int n = (int)cpu->regs.a - (int)cpu->current_data;
    cpu->regs.f_z = n == 0;
    cpu->regs.f_n = 1;
    cpu->regs.f_h = ((int)cpu->regs.a & 0xf) - ((int)cpu->current_data & 0xf) < 0;
    cpu->regs.f_c = n < 0;
}

void cb_instruction(Cpu* cpu) {
    static const int choice_regs[] = {
        [0] = REGISTER_NAME_B,
        [1] = REGISTER_NAME_C,
        [2] = REGISTER_NAME_D,
        [3] = REGISTER_NAME_E,
        [4] = REGISTER_NAME_H,
        [5] = REGISTER_NAME_L,
        [6] = REGISTER_NAME_HL,
        [7] = REGISTER_NAME_A,
    };

    static const CBInstructionLayout instruction_params[] = {
        [0x1] = {INSTRUCTION_CB_RLC},
        [0x2] = {INSTRUCTION_CB_RRC},
        [0x3] = {INSTRUCTION_CB_RL},
        [0x4] = {INSTRUCTION_CB_RR},
        [0x5] = {INSTRUCTION_CB_SLA},
        [0x6] = {INSTRUCTION_CB_SRA},
        [0x7] = {INSTRUCTION_CB_SWAP},
        [0x8] = {INSTRUCTION_CB_SRL},
        [0x9] = {INSTRUCTION_CB_BIT, 0},
        [0xa] = {INSTRUCTION_CB_BIT, 1},
        [0xb] = {INSTRUCTION_CB_BIT, 2},
        [0xc] = {INSTRUCTION_CB_BIT, 3},
        [0xd] = {INSTRUCTION_CB_BIT, 4},
        [0xe] = {INSTRUCTION_CB_BIT, 5},
        [0xf] = {INSTRUCTION_CB_BIT, 6},
        [0x10] = {INSTRUCTION_CB_RES, 7},
        [0x11] = {INSTRUCTION_CB_RES, 0},
        [0x12] = {INSTRUCTION_CB_RES, 1},
        [0x13] = {INSTRUCTION_CB_RES, 2},
        [0x14] = {INSTRUCTION_CB_RES, 3},
        [0x15] = {INSTRUCTION_CB_RES, 4},
        [0x16] = {INSTRUCTION_CB_RES, 5},
        [0x17] = {INSTRUCTION_CB_RES, 6},
        [0x18] = {INSTRUCTION_CB_RES, 7},
        [0x19] = {INSTRUCTION_CB_RES, 7},
        [0x1a] = {INSTRUCTION_CB_SET, 0},
        [0x1b] = {INSTRUCTION_CB_SET, 1},
        [0x1c] = {INSTRUCTION_CB_SET, 2},
        [0x1d] = {INSTRUCTION_CB_SET, 3},
        [0x1e] = {INSTRUCTION_CB_SET, 4},
        [0x1f] = {INSTRUCTION_CB_SET, 5},
        [0x20] = {INSTRUCTION_CB_SET, 6},
        [0x21] = {INSTRUCTION_CB_SET, 7},
    };

    uint8_t instruction = cpu->current_data;
    uint8_t reg_name  = REGISTER_NAME_NONE;

    // can use bit masking instead of condition...
    if ((instruction & 0x0f) <= 0x7) {
        reg_name = choice_regs[instruction & 0xf];
    } else {
        reg_name = choice_regs[(instruction & 0xf)  - 0x8];
    }

    cpu->cycles++;
    if (reg_name == REGISTER_NAME_HL) {
        cpu->cycles+=2;
    }

    CBInstructionLayout instruction_layout =
        instruction_params[
            (((instruction >> 4) & 0xf) * 2)
                + (uint8_t)((instruction & 0x0f) > 0x7)
        ];

    switch(instruction_layout.type) {
        case INSTRUCTION_CB_RLC:
            rlc_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_RRC:
            rrc_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_RL:
            rl_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_RR:
            rr_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_SLA:
            sla_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_SRA:
            sra_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_SWAP:
            swap_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_SRL:
            srl_instruction(cpu, reg_name);
            break;
        case INSTRUCTION_CB_BIT:
            bit_instruction(cpu, reg_name, instruction_layout.param);
            break;
        case INSTRUCTION_CB_RES:
            resset_instruction(cpu, reg_name, instruction_layout.param, 0);
            break;
        case INSTRUCTION_CB_SET:
            resset_instruction(cpu, reg_name, instruction_layout.param, 1);
            break;
        default:
            fprintf(stderr, "UNKOWN CB INSTRUCTION\n");
            exit(1);
    }
}

void rlc_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value << 1) + cpu->regs.f_c);
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name)) == 0;
    } else {
        set_reg(cpu, reg_name, (value << 1) + cpu->regs.f_c);
        cpu->regs.f_z = get_reg(cpu, reg_name) == 0;
    }
}

void rrc_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;
    cpu->regs.f_c = (value >> 7) & 0x1;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value >> 1) | (cpu->regs.f_c << 7));
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name)) == 0;
    } else {
        set_reg(cpu, reg_name, (value >> 1) | (cpu->regs.f_c << 7));
        cpu->regs.f_z = get_reg(cpu, reg_name) == 0;
    }
}

void rl_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value << 1) + cpu->regs.f_c);
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name)) == 0;
        cpu->regs.f_c = ((mmu_read(cpu, get_reg(cpu, reg_name)) << 7) >> 7) & 0x1;
    } else {
        set_reg(cpu, reg_name, (value << 1) + cpu->regs.f_c);
        cpu->regs.f_z = get_reg(cpu, reg_name) == 0;
        cpu->regs.f_c = (get_reg(cpu, reg_name) >> 7) & 0x1;
    }
}

void rr_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value >> 1) | (cpu->regs.f_c << 7));
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name));
        cpu->regs.f_c = (mmu_read(cpu, get_reg(cpu, reg_name)) >> 7) & 0x1;
    } else {
        set_reg(cpu, reg_name, (value >> 1) | (cpu->regs.f_c << 7));
        cpu->regs.f_z = get_reg(cpu, reg_name);
        cpu->regs.f_c = (get_reg(cpu, reg_name) >> 7) & 0x1;
    }

}

void sla_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;
    cpu->regs.f_c = ((value << 7) >> 7) & 0x1;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value << 1));
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name)) == 0;
    } else {
        set_reg(cpu, reg_name, value << 1);
        cpu->regs.f_z = get_reg(cpu, reg_name) == 0;
    }
}

void sra_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;
    cpu->regs.f_c = value & 0x1;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value >> 1));
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name)) == 0;
    } else {
        set_reg(cpu, reg_name, value >> 1);
        cpu->regs.f_z = get_reg(cpu, reg_name) == 0;
    }
}

void swap_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;
    uint8_t hi = (value >> 4) & 0xf;
    uint8_t lo = value & 0xf;
    value  = (lo << 4) | hi;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), value);
    } else {
        set_reg(cpu, reg_name, value);
    }
}

void srl_instruction(Cpu* cpu, int reg_name) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), (value >> 1));
        cpu->regs.f_z = mmu_read(cpu, get_reg(cpu, reg_name)) == 0;
        cpu->regs.f_c = (value & 0x1);
    } else {
        set_reg(cpu, reg_name, value >> 1);
        cpu->regs.f_z = get_reg(cpu, reg_name) == 0;
        cpu->regs.f_c = value & 0x1;
    }
}

void bit_instruction(Cpu* cpu, int reg_name, uint8_t bit) {
    uint8_t value;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }
    cpu->regs.f = 0;
    cpu->regs.f_z = ((value & (1 << bit)) >> bit) & 0x1;
    cpu->regs.f_n = 1;
}

void resset_instruction(Cpu* cpu, int reg_name, uint8_t bit, uint8_t set_or_reset) {
    uint8_t value;
    register uint8_t mask;
    if (reg_name == REGISTER_NAME_HL) {
        value = mmu_read(cpu, get_reg(cpu, reg_name));
    } else {
        value = get_reg(cpu, reg_name);
    }

    if (set_or_reset) {
        value = (value | (1 << bit));
    } else {
        mask = (1 << bit) ^ 0xff;
        value &= mask;
    }

    if (reg_name == REGISTER_NAME_HL) {
        mmu_write(cpu, get_reg(cpu, reg_name), value);
    } else {
        set_reg(cpu, reg_name, value);
    }
}

void rlca_instruction(Cpu* cpu) {
    uint8_t bit7 = (((cpu->regs.a) << 7) >> 7) & 0x1;
    cpu->regs.a <<= 1;
    cpu->regs.a |= bit7;
    cpu->regs.f = 0;
    cpu->regs.f_c = bit7;
}

void rrca_instruction(Cpu* cpu) {
    uint8_t bit0 = cpu->regs.a & 0x1;
    cpu->regs.a >>= 1;
    cpu->regs.a |= (bit0 << 7);
    cpu->regs.f = 0;
    cpu->regs.f_c = bit0;
}

void rla_instruction(Cpu* cpu) {
    uint8_t bit7 = (((cpu->regs.a) << 7) >> 7) & 0x1;
    cpu->regs.a <<= 1;
    cpu->regs.a |= cpu->regs.f_c;
    cpu->regs.f = 0;
    cpu->regs.f_c = bit7;
}

void rra_instruction(Cpu* cpu) {
    uint8_t bit0 = cpu->regs.a & 0x1;
    cpu->regs.a >>= 1;
    cpu->regs.a |= cpu->regs.f_c;
    cpu->regs.f = 0;
    cpu->regs.f_c = bit0;
}

void daa_instruction(Cpu* cpu) {
    uint8_t result = cpu->regs.a;
    uint8_t correction = 0;
    correction |= cpu->regs.f_h ? 0x06 : 0x00;
    correction |= cpu->regs.f_c ? 0x60 : 0x00;
    if (cpu->regs.f_n) {
        result -= correction;
    } else {
        correction |= ((result & 0x0F) > 0x09) ? 0x06 : 0x00;
        correction |= (result > 0x99) ? 0x60 : 0x00;
        result += correction;
    }
    cpu->regs.f_z = result == 0;
    cpu->regs.f_c = ((correction & 0x60) != 0);
    cpu->regs.a = result;
}

void cpl_instruction(Cpu* cpu) {
    cpu->regs.a = ~cpu->regs.a;
    cpu->regs.f_n = 1;
    cpu->regs.f_h = 1;
}

void scf_instruction(Cpu* cpu) {
    cpu->regs.f_n = 0;
    cpu->regs.f_h = 0;
    cpu->regs.f_c = 1;
}

void ccf_instruction(Cpu* cpu) {
    cpu->regs.f_n = 0;
    cpu->regs.f_h = 0;
    cpu->regs.f_c = !cpu->regs.f_c;
}

