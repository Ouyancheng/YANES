#pragma once

#include "sdk.h"


/** rom base */
struct nesrom {
    uint8_t *rom_data;   // a pointer to the rom data 
    unsigned PRG_rom_offset;
    unsigned PRG_rom_size;
    unsigned CHR_rom_size;
    uint32_t mapper_id;
};
struct nescpu;
struct nesppu;
typedef uint8_t (*PRG_reader_t)(struct nescpu *cpu, struct nesrom *rom, uint16_t addr);
typedef void (*PRG_writer_t)(struct nescpu *cpu, struct nesrom *rom, uint16_t addr, uint8_t value);

typedef uint8_t (*CHR_reader_t)(struct nesppu *ppu, struct nesrom *rom, uint16_t addr);
typedef void (*CHR_writer_t)(struct nesppu *ppu, struct nesrom *rom, uint16_t addr, uint8_t value);






