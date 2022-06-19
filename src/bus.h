#pragma once
#include "sdk.h"
#include "rom.h"
enum nametable_mirror {
    MIRROR_VERT, 
    MIRROR_HORI, 
    MIRROR_FOUR, 
};
struct nesbus {
    uint8_t cpu_ram[2048];
    unsigned cycles_elapsed;
    enum nametable_mirror mirroring;
    mapper_reader_t PRG_reader;
    mapper_writer_t PRG_writer;
    mapper_reader_t CHR_reader;
    mapper_writer_t CHR_writer; 
    struct nesrom *rom;
};
void bus_init(struct nesbus *bus); 
uint8_t bus_peek8(struct nesbus *bus, uint16_t addr); 
uint8_t bus_read8(struct nesbus *bus, uint16_t addr); 
void bus_write8(struct nesbus *bus, uint16_t addr, uint8_t value); 



void bus_catchup_cycles(struct nesbus *bus, unsigned cycles); 

void bus_load_rom(struct nesbus *bus, void *TODO);


