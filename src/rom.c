#include "rom.h"
#include "mappers/mapper0.h"
#include "bus.h"


struct nesrom *create_mapper_ines(uint32_t mapper_id, uint8_t *data, struct nesbus *bus) {
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
            bus->PRG_reader = &NROM_read_PRG;
            bus->PRG_writer = &NROM_write_PRG;
            bus->CHR_reader = &NROM_read_CHR;
            bus->CHR_writer = &NROM_write_CHR;
            rom->mapper_id = mapper_id;
            break;
        default:
            return NULL;
    }
    rom->rom_data = data;
    bus->mirroring = mirroring;
    rom->PRG_rom_size = PRG_size;
    rom->CHR_rom_size = CHR_size;
    rom->PRG_rom_offset = PRG_offset;
    bus->rom = rom;
    // printf("rom = %p\n", rom);
    return rom;
}

int load_rom_from_ines_data(struct nesbus *bus, uint8_t *data, size_t data_size) {
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
    struct nesrom *mapper_ptr = create_mapper_ines(mapper_id, data, bus); 
    if (mapper_ptr == NULL) {
        printf("unsupported mapper id %d\n", mapper_id);
        return -4;
    }
    return 0; 
}


void rom_deallocate(struct nesrom *rom) {
    if (!rom) return;
    if (rom->rom_data) {
        deallocate_filedata(rom->rom_data);
    }
    deallocate_memory(rom); 
}



