#include "cpu.h"


extern inline uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value); 
extern inline uint16_t cpu_peek16(struct nescpu *cpu, uint16_t addr);

/** https://www.nesdev.org/wiki/CPU_power_up_state */
void cpu_init(struct nescpu *cpu, struct nesppu *ppu) {
    cpu->a = 0; 
    cpu->x = 0; 
    cpu->y = 0; 
    cpu->sp = CPU_POWERUP_SP; 
    // 0b0011_0100 or 0x34
    cpu->p = (uint8_t)CPU_FLAG__ | (uint8_t)CPU_FLAG_I; // (uint8_t)CPU_FLAG_B | 
    cpu->pc = 0; // ? 
    memset(cpu->ram, 0, 2048);
    cpu->cpu_bus_last_value = 0;
    cpu->ppu = ppu;
    cpu->rom = NULL;
    cpu->PRG_reader = NULL;
    cpu->PRG_writer = NULL;
}

/** 
 * https://www.nesdev.org/wiki/CPU_power_up_state 
 * https://www.pagetable.com/?p=410 
 */
void cpu_reset(struct nescpu *cpu) {
    // DO NOT call the init procedure again!!!
    cpu->sp = CPU_POWERUP_SP; // it seems like it's 0x00 - 3 = 0xFD, not 0xFD - 3... 
    cpu->p |= CPU_FLAG_I;
    cpu->pc = cpu_read16(cpu, CPU_RESET_VECTOR); 
}


uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr) {
    uint8_t value;
    if (addr < 0x2000) {
        value = cpu->ram[addr % 0x0800]; // note: 0x0800 = 2048
    } else if (addr < 0x4000) {
        addr = 0x2000 + ((addr - 0x2000) % 8);
        value = ppu_external_read8(cpu->ppu, addr);
    } else if (addr < 0x4018) { 
        /// APU registers and IO registers 
        /// TODO: APU and controller
        value = 0xff;
    } else { 
        /// Cartridge space 
        value = (*(cpu->PRG_reader))(cpu, cpu->ppu, cpu->rom, addr); 
    }
    cpu->cpu_bus_last_value = value;
    return value;
}
void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value) {
    cpu->cpu_bus_last_value = value;
    if (addr < 0x2000) {
        cpu->ram[addr % 0x0800] = value; 
    } else if (addr < 0x4000) {
        addr = 0x2000 + ((addr - 0x2000) % 8);
        ppu_external_write8(cpu->ppu, addr, value);
    } else if (addr < 0x4018) { 
        /// APU registers and IO registers 
        /// TODO: APU and controller 
        return;
    } else { 
        /// Cartridge space 
        (*(cpu->PRG_writer))(cpu, cpu->ppu, cpu->rom, addr, value); 
    }
}

uint8_t cpu_peek8(struct nescpu *cpu, uint16_t addr) {
    if (addr < 0x2000) {
        return cpu->ram[addr % 0x0800]; 
    } else if (addr < 0x4000) {
        addr = 0x2000 + ((addr - 0x2000) % 8);
        return ppu_external_peek8(cpu->ppu, addr);
    } else if (addr < 0x4018) { 
        /// APU registers and IO registers 
        /// TODO: APU and controller 
        return 0xff;
    } else { 
        /// Cartridge space 
        return (*(cpu->PRG_reader))(cpu, cpu->ppu, cpu->rom, addr); 
    }
}



