#ifndef STACK_OPERATIONS_H
#define STACK_OPERATIONS_H

void stack_push(Cpu* cpu, uint8_t value);
void stack_push16(Cpu* cpu, uint16_t value);
uint8_t stack_pop(Cpu* cpu);
uint16_t stack_pop16(Cpu* cpu);

#endif
