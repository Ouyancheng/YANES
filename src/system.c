#include "system.h"
#include "mappers/mapper0.h"

void system_init(struct nessystem *sys) {
    ppu_init(&(sys->ppu));
    cpu_init(&(sys->cpu), &(sys->ppu));
    sys->rom = NULL;
}
int system_run(struct nessystem *sys) {
    if (!sys->rom) {
        printf("rom not loaded!\n");
        return -1;
    }
    cpu_run(&(sys->cpu), NULL); 
    return 0;
}

int system_test_run(struct nessystem *sys, uint16_t cpu_init_pc, void (*cpu_callback)(struct nescpu *cpu)) {
    if (!sys->rom) {
        printf("rom not loaded!\n");
        return -1;
    }
    sys->cpu.pc = cpu_init_pc;
    cpu_run(&(sys->cpu), cpu_callback); 
    return 0;
}

struct nesrom *system_allocate_mapper_ines(struct nessystem *sys, uint32_t mapper_id, uint8_t *data, size_t data_size) {
    void *mapper;
    struct nesrom *rom;
    unsigned PRG_size = (unsigned)data[4] * 16 * 1024; 
    unsigned CHR_size = (unsigned)data[5] * 8 * 1024; 
    unsigned PRG_offset = (data[6] & 0b100) ? (16+512) : 16;
    enum nametable_mirror mirroring = (data[6] & 0b1000) ? MIRROR_FOUR : ((data[6] & 1) ? MIRROR_VERT : MIRROR_HORI);
    switch (mapper_id) {
        case 0:
            mapper = allocate_memory(sizeof(struct mapper0_NROM)); 
            if (!mapper) {
                printf("Failed to allocate memory\n");
                return NULL;
            }
            rom = (struct nesrom *)mapper;
            sys->cpu.PRG_reader = &NROM_read_PRG;
            sys->cpu.PRG_writer = &NROM_write_PRG;
            sys->ppu.CHR_reader = &NROM_read_CHR;
            sys->ppu.CHR_writer = &NROM_write_CHR;
            break;
        default:
            return NULL;
    }
    rom->mapper_id = mapper_id;
    rom->rom_data = data;
    rom->PRG_rom_size = PRG_size;
    rom->CHR_rom_size = CHR_size;
    rom->PRG_rom_offset = PRG_offset;
    ppu_set_nametable_mirror(&(sys->ppu), mirroring);
    sys->rom = rom;
    sys->cpu.rom = sys->rom;
    sys->ppu.rom = sys->rom;
    return rom;
}


/** loads and allocates rom from ines data */
int system_load_rom_ines(struct nessystem *sys, uint8_t *data, size_t data_size) {
    if (data_size < 16) {
        printf("Error: ines header at least 16 bytes, but the size of data is %lu", data_size);
        return -1;
    }
    uint8_t ines1_tag[] = {0x4E, 0x45, 0x53, 0x1A};
    if (memcmp(data, (uint8_t*)ines1_tag, sizeof(ines1_tag)) != 0) {
        printf("Error: load_rom not a valid nes rom!\n"); 
        return -2;
    }
    uint8_t version = (data[7] >> 2) & 0x3; 
    if (version != 0) {
        printf("Error: version %d is not supported\n", version); 
        return -3;
    }
    uint32_t mapper_id = (
        // (((uint32_t)data[8] & 0x0F) << 8) | 
        ((uint32_t)data[7] & 0xF0) | 
        (((uint32_t)data[6] & 0xF0) >> 4)
    ); 
    struct nesrom *mapper_ptr = system_allocate_mapper_ines(sys, mapper_id, data, data_size); 
    if (mapper_ptr == NULL) {
        printf("unsupported mapper id %d\n", mapper_id);
        return -4;
    }
    /// NOTE: inserting a new rom should trigger a reset interrupt to the CPU
    cpu_reset(&(sys->cpu)); 
    ppu_reset(&(sys->ppu));
    return 0; 
}

/** unloads the rom and its data */
void system_unload_rom(struct nessystem *sys) {
    if (!sys->rom) return;
    if (sys->rom->rom_data) {
        deallocate_filedata(sys->rom->rom_data);
    }
    deallocate_memory(sys->rom); 
    sys->rom = NULL;
    sys->cpu.rom = NULL;
    sys->ppu.rom = NULL;
}

void system_reset(struct nessystem *sys) {
    /// is this correct?
    cpu_reset(&(sys->cpu)); 
    ppu_reset(&(sys->ppu));
}

