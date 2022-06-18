#pragma once

#include "sdk.h"

#define ROM_SIZE_MAX 1024*1024
#define MAPPER_STRUCT_SIZE_MAX 64

enum nametable_mirror {
    MIRROR_VERT, 
    MIRROR_HORI, 
    MIRROR_FOUR, 
};

struct nesrom {
    void    *mapper_ptr; // this is a pointer to the mapper struct 
    uint8_t (*mapper_read_PRG)(struct nesrom *rom, uint16_t addr); 
    void    (*mapper_write_PRG)(struct nesrom *rom, uint16_t addr, uint8_t value); 
    uint8_t (*mapper_read_CHR)(struct nesrom *rom, uint16_t addr); 
    void    (*mapper_write_CHR)(struct nesrom *rom, uint16_t addr, uint8_t value); 
    uint8_t *PRG_window; // the PRG rom pointer
    uint8_t *CHR_window; // the CHR rom pointer
    uint8_t *rom_data;   // a pointer to the rom data 
    unsigned PRG_rom_offset;
    unsigned PRG_rom_size;
    unsigned CHR_rom_size;
    enum nametable_mirror mirroring;
    unsigned mapper_id;
};


int load_rom_from_ines_data(struct nesrom *rom, uint8_t *data); 
void *create_mapper(struct nesrom *rom, unsigned mapper_id);

void rom_reset(struct nesrom *rom);



