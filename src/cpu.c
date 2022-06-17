/**
 * The NES's 6502 CPU 
 * Very useful information: 
 * Opcode description: https://llx.com/Neil/a2/opcodes.html 
 * The 6502 instruction set: https://www.masswerk.at/6502/6502_instruction_set.html 
 * The illegal opcodes: https://www.masswerk.at/nowgobang/2021/6502-illegal-opcodes 
 * The opcode table: https://www.nesdev.org/wiki/CPU_unofficial_opcodes 
 * The 6502 simulator: http://www.visual6502.org/JSSim/expert.html 
 * The instructions: http://www.6502.org/tutorials/6502opcodes.html 
 */

#include "cpu.h"
#include "cpu_opcodes.h"


extern inline uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value); 

extern inline uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value); 


/** https://www.nesdev.org/wiki/CPU_power_up_state */
void cpu_init(struct nescpu *cpu) {
    cpu->a = 0; 
    cpu->x = 0; 
    cpu->y = 0; 
    cpu->sp = CPU_POWERUP_SP; 
    // 0b0011_0100 or 0x34
    cpu->p = (uint8_t)CPU_FLAG__ | (uint8_t)CPU_FLAG_B | (uint8_t)CPU_FLAG_I; 
    cpu->pc = 0; // ? 
}

/** 
 * https://www.nesdev.org/wiki/CPU_power_up_state 
 * https://www.pagetable.com/?p=410 
 */
void cpu_reset(struct nescpu *cpu) {
    cpu_init(cpu); // technically speaking this is not quite right, we don't need this
    cpu->sp = CPU_POWERUP_SP; // it seems like it's 0x00 - 3 = 0xFD, not 0xFD - 3... 
    set_flag(cpu, CPU_FLAG_I, 1); 
    cpu->pc = cpu_read16(cpu, CPU_RESET_VECTOR); 
}



