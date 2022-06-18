#include "rom.h"
#include "mappers/mapper0.h"
int load_rom_from_ines_data(struct nesrom *rom, uint8_t *data) {
    uint8_t ines1_tag[] = {0x4E, 0x45, 0x53, 0x1A};
    if (memcmp(data, (uint8_t*)ines1_tag, sizeof(ines1_tag)) != 0) {
        printf("Error: load_rom not a valid nes rom!\n"); 
        return -1;
    }
    uint8_t version = (data[7] >> 2) & 0x3; 
    if (version != 0) {
        printf("Error: version %d is not supported\n", version); 
        return -2;
    }
    unsigned mapper_id = ((data[7] & 0xF0) | (data[6] >> 4)); 
    void *mapper_ptr = create_mapper(rom, mapper_id); 
    if (mapper_ptr == NULL) {
        printf("unsupported mapper id %d\n", rom->mapper_id);
        return -3;
    }
    
    rom->rom_data = data;
    uint8_t four_screen = data[6] & 0b1000;
    if (four_screen) {
        rom->mirroring = MIRROR_FOUR;
    } else {
        rom->mirroring = (data[6] & 1) ? MIRROR_VERT : MIRROR_HORI; 
    }
    rom->PRG_rom_size = (unsigned)data[4] * 16 * 1024; 
    rom->CHR_rom_size = (unsigned)data[5] * 8 * 1024; 

    if (data[6] & 0b100) {
        rom->PRG_rom_offset = 16 + 512;
    } else {
        rom->PRG_rom_offset = 16; 
    }
    rom->PRG_window = data + rom->PRG_rom_offset; 

    rom->CHR_window = rom->PRG_window + rom->PRG_rom_size; 
    return 0; 
}

void *create_mapper(struct nesrom *rom, unsigned mapper_id) {
    void *mapper;
    switch (mapper_id) {
        case 0:
            mapper = allocate_memory(sizeof(struct mapper0_NROM)); 
            if (!mapper) return NULL;
            rom->mapper_read_PRG = NROM_read_PRG;
            rom->mapper_write_PRG = NROM_write_PRG;
            rom->mapper_read_CHR = NROM_read_CHR;
            rom->mapper_write_CHR = NROM_write_CHR;
            rom->mapper_id = mapper_id;
            rom->mapper_ptr = mapper;
            return mapper;
        default:
            return NULL;
    }
}

void rom_reset(struct nesrom *rom) {
    if (rom->mapper_ptr) {
        deallocate_memory(rom->mapper_ptr);
    }
    if (rom->rom_data) {
        deallocate_memory(rom->rom_data);
    }
    memset(rom, 0, sizeof(struct nesrom));
}
