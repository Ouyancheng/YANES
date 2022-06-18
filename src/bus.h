#pragma once
#include "sdk.h"
struct nesbus {
    uint8_t cpu_ram[2048];
    unsigned cycles_elapsed;
};
void bus_init(struct nesbus *bus); 

uint8_t bus_peek8(struct nesbus *bus, uint16_t addr); 
uint8_t bus_read8(struct nesbus *bus, uint16_t addr); 
void bus_write8(struct nesbus *bus, uint16_t addr, uint8_t value); 




void bus_catchup_cycles(struct nesbus *bus, unsigned cycles); 

void bus_load_rom(struct nesbus *bus, void *TODO);


