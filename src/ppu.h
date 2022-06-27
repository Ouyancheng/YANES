#pragma once 

#include "sdk.h"
#include "rom.h"

enum nametable_mirror {
    MIRROR_VERT = 0, 
    MIRROR_HORI = 1, 
    MIRROR_FOUR = 2, 
};

enum ppu_registers {
    PPUCTRL   = 0x2000,
    PPUMASK   = 0x2001,
    PPUSTATUS = 0x2002,
    OAMADDR   = 0x2003,
    OAMDATA   = 0x2004,
    PPUSCROLL = 0x2005,
    PPUADDR   = 0x2006,
    PPUDATA   = 0x2007,
    OAMDMA    = 0x4014
};

/** Flags for PPUCTRL 0x2000 */
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
/** Flags for PPUMASK 0x2001 */
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
/** Flags for PPUSTATUS 0x2002 */
enum ppu_status_flags {
    PPUSTATUS_UNUSED1               = 0b00000001,
    PPUSTATUS_UNUSED2               = 0b00000010,
    PPUSTATUS_UNUSED3               = 0b00000100,
    PPUSTATUS_UNUSED4               = 0b00001000,
    PPUSTATUS_UNUSED5               = 0b00010000,
    PPUSTATUS_SPRITE_OVERFLOW       = 0b00100000,
    PPUSTATUS_SPRITE_ZERO_HIT       = 0b01000000,
    PPUSTATUS_VBLANK_STARTED        = 0b10000000 
};



enum ppu_state {
    PPUSTATE_RENDER = 0,
    PPUSTATE_POSTRENDER = 1,
    PPUSTATE_VBLANK = 2,
    PPUSTATE_PRERENDER = 3,
};
struct nesppu {
    /** the current vram address (15 bits) */
    uint16_t v;
    /** the temporary vram address (15 bits) */
    uint16_t t;
    /** fine x scroll (3 bits) */
    uint8_t x;
    /** first of second write toggle (1 bit) */
    uint8_t w; // notice that both the PPUSCROLL and the PPUADDR use the same toggle

    /**
     * The structure of t and v: 
     * 15   14               0
     * 0    yyy NN YYYYY XXXXX
     * 
     * yyy: fine y scroll -- the y-position within a tile 
     * NN: the nametable address -- basically the base address of the nametable - 0x2000 = {0x0000, 0x4000, 0x8000, 0xC000}
     * YYYYY: coarse Y scroll -- the y-position of the tile 
     * XXXXX: coarse X scroll -- the x-position of the tile 
     * 
     * xxx: fine x scroll -- similar to yyy
     * w: just to specify whether it's the first of the second write 
     */
    /** 
     * 2 16-bit shift registers - These contain the pattern table data for two tiles. 
     * Every 8 cycles, the data for the next tile is loaded into the upper 8 bits of this shift register. 
     * Meanwhile, the pixel to render is fetched from one of the lower 8 bits.
     */
    uint32_t background_pattern_table_data; 
    /**
     * The tile that's currently rendering
     */
    uint16_t current_background_tile;
    /**
     * 2 8-bit shift registers - These contain the palette attributes for the lower 8 pixels of the 16-bit shift register. 
     * These registers are fed by a latch which contains the palette attribute for the next tile. 
     * Every 8 cycles, the latch is loaded with the palette attribute for the next tile.
     */
    uint16_t background_palette_attributes;
    /**
     * Two nametable bytes used for fetching the tile data
     */
    uint16_t nametable_bytes;
    /**
     * 8 pairs of 8-bit shift registers - 
     * These contain the pattern table data for up to 8 sprites, to be rendered on the current scanline. 
     * Unused sprites are loaded with an all-transparent set of values.
     */
    uint8_t sprite_pattern_table_data[8][2];
    /**
     * 8 latches - These contain the attribute bytes for up to 8 sprites.
     */
    uint8_t sprite_attributes[8];
    /**
     * 8 counters - These contain the X positions for up to 8 sprites.
     */
    uint8_t sprite_x_counters[8];
    /**
     * Secondary OAM (holds 8 sprites for the current scanline)
     */
    uint8_t secondary_oam[32];

    /** PPUCTRL 0x2000 */
    uint8_t ctrl;
    /** PPUMASK 0x2001 */
    uint8_t mask;
    /** PPUSTATUS 0x2002 */
    uint8_t status;
    /** OAMADDR 0x2003 */
    uint8_t oamaddr; 

    /** the read buffer for the 1-cycle delay of reading VRAM */
    uint8_t data_read_buffer; 
    
    /** the bus to the ROM */
    struct nesrom *rom;
    /** function pointers to reading/writing CHR ROM */
    CHR_reader_t CHR_reader;
    CHR_writer_t CHR_writer; 

    /** For openbus reads */
    uint8_t iobus_last_value; 
    /** whether NMI is raised, if NMI is enabled */
    bool nmi_raised;
    /** the nametable mirroring type, could be customized though */
    enum nametable_mirror mirroring;
    /** the four nametables, use pointers here because sometimes the ROM provides extra memory for this */
    uint8_t *nametables[4];

    enum ppu_state state;
    /** number of frames rendered */
    uint32_t frames;
    /** cycles elasped */
    uint32_t cycles;
    /** current dot in the line / x coordinate */
    uint16_t dots;
    /** current line / y coordinate */
    uint16_t lines;
    uint8_t palette_ram[32];
    uint8_t oamdata[256];
    uint8_t vram[2048];
};

void ppu_init(struct nesppu *ppu);
void ppu_reset(struct nesppu *ppu); 
/** sets the nametable mirroring, and update the nametable pointers */
void ppu_set_nametable_mirror(struct nesppu *ppu, enum nametable_mirror mirroring);
/** the non-destructive read for PPU */
uint8_t ppu_external_peek8(struct nesppu *ppu, uint16_t addr);
/** performs a read from 0x2000 - 0x2007 */
uint8_t ppu_external_read8(struct nesppu *ppu, uint16_t addr);
/** performs a write from 0x2000 - 0x2007 */
void ppu_external_write8(struct nesppu *ppu, uint16_t addr, uint8_t value); 
/** performs a read on PPU's internal address space */
uint8_t ppu_internal_read8(struct nesppu *ppu, uint16_t addr); 
/** performs a write on PPU's external address space */
void ppu_internal_write8(struct nesppu *ppu, uint16_t addr, uint8_t value); 

/** Handle OAM DMA when writing to OAMDMA 0x4014, TODO: implement this */
void ppu_oam_dma(struct nesppu *ppu, struct nescpu *cpu, uint8_t page); 
/** tick the PPU by the number of PPU cycles */
void ppu_tick(struct nesppu *ppu, unsigned num_cycles);

