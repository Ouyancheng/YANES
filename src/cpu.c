#include "cpu.h"

void cpu_init(struct nescpu *cpu) {
    cpu->a = 0; 
    cpu->x = 0; 
    cpu->y = 0; 
    cpu->sp = CPU_SP_RESET; 
    cpu->p = (uint8_t)cpu_flag_break2 | (uint8_t)cpu_flag_interrupt_disable; 
    cpu->pc = 0; 
}


