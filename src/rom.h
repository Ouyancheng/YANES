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

typedef uint8_t (*mapper_reader_t)(struct nesrom *rom, uint16_t addr);
typedef void (*mapper_writer_t)(struct nesrom *rom, uint16_t addr, uint8_t value);
struct nesbus;
/** loads and allocates rom from ines data */
int load_rom_from_ines_data(struct nesbus *bus, uint8_t *data, size_t data_size); 
/** unloads the rom and its data */
void unload_rom(struct nesbus *bus); 




