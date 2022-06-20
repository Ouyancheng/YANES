#pragma once
#include "rom.h"
struct mapper0_NROM {
    struct nesrom rom;
    // uint32_t PRG_ram_size;
    // uint8_t PRG_ram[4096]; 
    // uint8_t CHR_ram[8192];
};
struct nescpu;
struct nesppu;
uint8_t NROM_read_PRG(struct nescpu *cpu, struct nesrom *rom, uint16_t addr); 
void NROM_write_PRG(struct nescpu *cpu, struct nesrom *rom, uint16_t addr, uint8_t value); 

uint8_t NROM_read_CHR(struct nesppu *ppu, struct nesrom *rom, uint16_t addr); 
void NROM_write_CHR(struct nesppu *ppu, struct nesrom *rom, uint16_t addr, uint8_t value); 

