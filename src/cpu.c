#include "cpu.h"


extern inline uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value); 

extern inline uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value); 


/** https://www.nesdev.org/wiki/CPU_power_up_state */
void cpu_init(struct nescpu *cpu, struct nesbus *bus) {
    cpu->a = 0; 
    cpu->x = 0; 
    cpu->y = 0; 
    cpu->sp = CPU_POWERUP_SP; 
    // 0b0011_0100 or 0x34
    cpu->p = (uint8_t)CPU_FLAG__ | (uint8_t)CPU_FLAG_I; // (uint8_t)CPU_FLAG_B | 
    cpu->pc = 0; // ? 
    cpu->bus = bus;
}

/** 
 * https://www.nesdev.org/wiki/CPU_power_up_state 
 * https://www.pagetable.com/?p=410 
 */
void cpu_reset(struct nescpu *cpu) {
    cpu_init(cpu, cpu->bus); // technically speaking this is not quite right, we don't need this
    cpu->sp = CPU_POWERUP_SP; // it seems like it's 0x00 - 3 = 0xFD, not 0xFD - 3... 
    cpu->p |= CPU_FLAG_I;
    cpu->pc = cpu_read16(cpu, CPU_RESET_VECTOR); 
}



