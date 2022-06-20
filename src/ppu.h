#pragma once

#include "sdk.h"
#include "rom.h"

enum nametable_mirror {
    MIRROR_VERT, 
    MIRROR_HORI, 
    MIRROR_FOUR, 
};
struct nesppu {
    uint8_t ctrl;
    uint8_t mask;
    uint8_t status;
    uint8_t oamaddr;
    bool scroll_latching_y;
    uint8_t scroll_x; /// Special: (two writes: X scroll, Y scroll)
    uint8_t scroll_y;
    bool addr_latching_lsb;
    uint16_t addr; /// Special: (two writes: most significant byte, least significant byte)

    uint8_t data_read_buffer; /// for PPU's one cycle delay of data
    enum nametable_mirror mirroring;
    

    struct nesrom *rom;
    CHR_reader_t CHR_reader;
    CHR_writer_t CHR_writer; 
    
    uint8_t iobus_last_value;
    uint8_t vmembus_last_value; 

    uint8_t *nametables[4];
    
    uint8_t palette_ram[32];
    uint8_t oamdata[256];
    uint8_t vram[2048];
};

enum ppu_status_flags {
    PPUSTATUS_UNUSED1         = 0b00000001,
    PPUSTATUS_UNUSED2         = 0b00000010,
    PPUSTATUS_UNUSED3         = 0b00000100,
    PPUSTATUS_UNUSED4         = 0b00001000,
    PPUSTATUS_UNUSED5         = 0b00010000,
    PPUSTATUS_SPRITE_OVERFLOW = 0b00100000,
    PPUSTATUS_SPRITE_ZERO_HIT = 0b01000000,
    PPUSTATUS_VBLANK_STARTED  = 0b10000000 
};

void ppu_init(struct nesppu *ppu);
void ppu_reset(struct nesppu *ppu); 




void ppu_set_nametable_mirror(struct nesppu *ppu, enum nametable_mirror mirroring);
uint8_t ppu_external_peek8(struct nesppu *ppu, uint16_t addr);
uint8_t ppu_external_read8(struct nesppu *ppu, uint16_t addr);
void ppu_external_write8(struct nesppu *ppu, uint16_t addr, uint8_t value); 

/// https://www.nesdev.org/wiki/PPU_memory_map 
uint8_t ppu_internal_read8(struct nesppu *ppu, uint16_t addr); 
void ppu_internal_write8(struct nesppu *ppu, uint16_t addr, uint8_t value); 
