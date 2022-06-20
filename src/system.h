#pragma once

#include "cpu.h"
#include "ppu.h"
#include "rom.h"

struct nessystem {
    struct nescpu cpu;
    struct nesppu ppu;
    struct nesrom *rom;
};


/** loads and allocates rom from ines data */
int system_load_rom_ines(struct nessystem *sys, uint8_t *data, size_t data_size); 

/** unloads the rom and its data */
void system_unload_rom(struct nessystem *sys); 

/** allocates the mapper according to mapper id and ines data */
struct nesrom *system_allocate_mapper_ines(struct nessystem *sys, uint32_t mapper_id, uint8_t *data, size_t data_size);


void system_init(struct nessystem *sys);
int system_run(struct nessystem *sys);
int system_test_run(struct nessystem *sys, uint16_t cpu_init_pc, void (*cpu_callback)(struct nescpu *cpu)); 

void system_reset(struct nessystem *sys);
