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
    bool nmi_raised;

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

enum ppu_control_flags {
    PPUCTRL_NAMETABLE1              = 0b00000001, 
    PPUCTRL_NAMETABLE2              = 0b00000010, 
    PPUCTRL_VRAM_ADDR_INCREMENT     = 0b00000100, 
    PPUCTRL_SPRITE_PATTERN_ADDR     = 0b00001000, 
    PPUCTRL_BACKGROUND_PATTERN_ADDR = 0b00010000, 
    PPUCTRL_SPRITE_SIZE             = 0b00100000, 
    PPUCTRL_MASTER_SLAVE_SELECT     = 0b01000000, 
    PPUCTRL_GENERATE_NMI            = 0b10000000 
};

enum ppu_mask_flags {
    PPUMASK_GREYSCALE               = 0b00000001,
    PPUMASK_LEFTMOST_8PX_BACKGROUND = 0b00000010,
    PPUMASK_LEFTMOST_8PX_SPRITE     = 0b00000100,
    PPUMASK_SHOW_BACKGROUND         = 0b00001000,
    PPUMASK_SHOW_SPRITES            = 0b00010000,
    PPUMASK_EMPHASIZE_RED           = 0b00100000,
    PPUMASK_EMPHASIZE_GREEN         = 0b01000000,
    PPUMASK_EMPHASIZE_BLUE          = 0b10000000
};

void ppu_init(struct nesppu *ppu);
void ppu_reset(struct nesppu *ppu); 




void ppu_set_nametable_mirror(struct nesppu *ppu, enum nametable_mirror mirroring);
uint8_t ppu_external_peek8(struct nesppu *ppu, uint16_t addr);
/// Be super careful on implementing this: https://www.nesdev.org/wiki/PPU_registers
uint8_t ppu_external_read8(struct nesppu *ppu, uint16_t addr);
void ppu_external_write8(struct nesppu *ppu, uint16_t addr, uint8_t value); 

/// https://www.nesdev.org/wiki/PPU_memory_map 
uint8_t ppu_internal_read8(struct nesppu *ppu, uint16_t addr); 
void ppu_internal_write8(struct nesppu *ppu, uint16_t addr, uint8_t value); 

void ppu_oam_dma(struct nesppu *ppu, struct nescpu *cpu, uint8_t page); /// TODO: implement this


