#include "ppu.h"
#include "cpu.h"

/** 
 * a tick that does nothing but increment the dots and possibly the lines 
 * (maybe) better not to inline it because we may want less branches to relief the branch predictor 
 */
static void idle_tick(struct nesppu *ppu) {
    if (ppu->dots == 340) {
        ppu->dots = 0;
        ppu->lines += 1;
    } else {
        ppu->dots += 1;
    }
}
static inline uint8_t fetch_nametable_byte(struct nesppu *ppu) {
    // fetch the nametable byte according to NN YYYYY XXXXX (nametable, coarse Y, coarse X), no fine y
    uint16_t tile_address = (UINT16_C(0x2000) | (ppu->v & UINT16_C(0x0FFF)));
    unsigned nametable_index = (tile_address - 0x2000) / 0x0400;
    uint8_t nametable_byte = ppu->nametables[nametable_index][tile_address - (0x2000 + 0x0400 * nametable_index)];
    return nametable_byte;
}
/** https://www.nesdev.org/wiki/PPU_scrolling#Tile_and_attribute_fetching */
static inline uint8_t fetch_attribute_byte(struct nesppu *ppu) {
    /*
    From the above link: 
    NN 1111 YYY XXX
    || |||| ||| +++-- high 3 bits of coarse X (x/4)
    || |||| +++------ high 3 bits of coarse Y (y/4)
    || ++++---------- attribute offset (960 bytes)
    ++--------------- nametable select
    */
    uint16_t attribute_address  = 0x23C0 // attribute table base address (at the end of the corresponding nametable)
                                | (ppu->v & 0x0C00) // nametable index 
                                | ((ppu->v >> 4) & 0x38) // 0b111000XXXX, the high 3 bits of coarse Y
                                | ((ppu->v >> 2) & 0x07); // 0b00000111XX, the high 3 bits of coarse X
    unsigned nametable_index = (attribute_address - 0x2000) / 0x0400;
    uint8_t attribute_byte = ppu->nametables[nametable_index][attribute_address - (0x2000 + 0x0400 * nametable_index)];
    return attribute_byte;
}
/** https://www.nesdev.org/wiki/PPU_scrolling#Coarse_X_increment */
static void coarse_X_increment(struct nesppu *ppu) {
    /*
    fine X value does not change during rendering; 
    the only thing that changes it is a $2005 first write
    */
    if ((ppu->v & 0b11111) == 0b11111) { // coarse X 
        ppu->v &= ~UINT16_C(0b11111);
        ppu->v ^= 0x0400; // switch horizontal nametable
    } else {
        ppu->v += 1;
    }
}
/** https://www.nesdev.org/wiki/PPU_scrolling#Y_increment */
static void y_increment(struct nesppu *ppu) {
    /*
    If rendering is enabled, fine Y is incremented at dot 256 of each scanline, 
    overflowing to coarse Y, and finally adjusted to wrap among the nametables vertically.
    */
    if ((ppu->v & 0x7000) != 0x7000) {  // if the fine-y is not-yet wrapping
        ppu->v += 0x1000;
    } else {
        ppu->v &= ~UINT16_C(0x7000);
        uint16_t coarse_Y = ((ppu->v >> 5) & 0b11111);
        if (coarse_Y == 29) { // Row 29 is the last row of tiles in a nametable.
            coarse_Y = 0;
            ppu->v ^= 0x0800; // switch vertical nametable
        } else if (coarse_Y == 31) {
            coarse_Y = 0; // coarse Y = 0, nametable not switched
        } else {
            coarse_Y += 1;
        }
        ppu->v = ((ppu->v & (~UINT16_C(0x03E0))) | (coarse_Y << 5)); // put coarse Y back into v
    }
}
/** fetch memory according to the memory timing during the rendering */
static void render_memory_fetch(struct nesppu *ppu) {
    unsigned dots_m_1 = ppu->dots - 1;
    switch (dots_m_1 % 8) {
        case 1: {
            // fetch nametable byte
            /** https://www.nesdev.org/wiki/PPU_nametables */
            uint16_t nametable_byte = fetch_nametable_byte(ppu);
            ppu->nametable_bytes >>= 8;
            ppu->nametable_bytes |= ((uint16_t)nametable_byte) << 8;
            break;
        }
        case 3: {
            // fetch attribute table byte
            /** https://www.nesdev.org/wiki/PPU_attribute_tables */
            uint16_t attribute_table_byte = fetch_attribute_byte(ppu);
            ppu->background_palette_attributes >>= 8;
            ppu->background_palette_attributes |= ((uint16_t)attribute_table_byte) << 8;
            break;
        }
        case 5: {
            // fetch lower background tile byte
            /** https://www.nesdev.org/wiki/PPU_pattern_tables */
            uint16_t nametable_byte = (ppu->nametable_bytes & 0xFF);
            uint16_t chr_address = (nametable_byte << 4) + ((ppu->v >> 12) & 0b111); // fine-y scroll
            chr_address |= ((ppu->ctrl & PPUCTRL_BACKGROUND_PATTERN_ADDR) >> 4) << 12;
            uint16_t lower_tile_byte = ppu->CHR_reader(ppu, ppu->rom, chr_address);
            ppu->background_pattern_table_data[0] >>= 8;
            ppu->background_pattern_table_data[0] |= ((uint16_t)lower_tile_byte) << 8;
            break;
        }
        case 7: {
            // fetch higher background tile byte and increment the horizontal v
            uint16_t nametable_byte = (ppu->nametable_bytes & 0xFF);
            uint16_t chr_address = (nametable_byte << 4) + ((ppu->v >> 12) & 0b111) + 8; // +8 bytes higher than the lower byte
            chr_address |= ((ppu->ctrl & PPUCTRL_BACKGROUND_PATTERN_ADDR) >> 4) << 12;
            uint16_t higher_tile_byte = ppu->CHR_reader(ppu, ppu->rom, chr_address);
            ppu->background_pattern_table_data[0] >>= 8;
            ppu->background_pattern_table_data[0] |= ((uint16_t)higher_tile_byte) << 8;
            if (ppu->dots == 256) {
                y_increment(ppu);
            } else{
                coarse_X_increment(ppu);
            }
            break;
        } 
        default:
            break;
    }
}
/** tick the ppu by one ppu cycle, or by num_cycles? */
void ppu_tick(struct nesppu *ppu, unsigned num_cycles) {
    ppu->cycles += 1;
    if (ppu->lines < 240) { // rendering
        /** TODO: handle not rendering behaviour correctly */
        // if (!(ppu->mask & PPUMASK_SHOW_BACKGROUND) && !(ppu->mask & PPUMASK_SHOW_SPRITES)) {
        //     idle_tick(ppu);
        //     return;
        // }
        // TODO: render? 
        if (ppu->dots == 0) {
            ppu->dots += 1;
        }
        else if (ppu->dots <= 256) {
            render_memory_fetch(ppu);
            ppu->dots += 1;
        }
        else if (ppu->dots == 257) {
            // hori(v) = hori(t)
            ppu->v &= ~UINT16_C(0b0000010000011111);
            ppu->v |= (ppu->t & 0b0000010000011111);
            ppu->dots += 1;
        }
        else if (ppu->dots <= 320) {
            ppu->dots += 1;
        }
        else {
            render_memory_fetch(ppu);
            if (ppu->dots == 340) {
                ppu->dots = 0;
                ppu->lines += 1;
            } else {
                ppu->dots += 1;
            }
        }
    }
    else if (ppu->lines < 260) { // vblank + // post render
        idle_tick(ppu);
        // set the vblank on the second tick otherwise idle
        if (ppu->lines == 241 && ppu->dots == 1) { 
            ppu->nmi_raised = true; 
        }
    }
    else { // pre-render
        if (ppu->dots == 0) {
            ppu->dots = 1;
        } 
        else if (ppu->dots == 1) {
            // clear vblank, overflow, sprite zero
            ppu->status &= ~(PPUSTATUS_VBLANK_STARTED | PPUSTATUS_SPRITE_OVERFLOW | PPUSTATUS_SPRITE_ZERO_HIT);
            ppu->nmi_raised = false;
            ppu->dots += 1;
        }
        else if (ppu->dots <= 256) {
            render_memory_fetch(ppu);
            ppu->dots += 1;
        }
        else if (ppu->dots == 257) {
            ppu->v &= ~UINT16_C(0b0000010000011111);
            ppu->v |= (ppu->t & 0b0000010000011111);
            ppu->dots += 1;
        }
        else if (ppu->dots <= 279) {
            ppu->dots += 1;
        }
        else if (ppu->dots <= 304) {
            // vert(v) = vert(t) at each tick
            ppu->v &= ~UINT16_C(0b0111101111100000);
            ppu->v |= (ppu->t & 0b0111101111100000);
            ppu->dots += 1;
        }
        else if (ppu->dots <= 320) {
            ppu->dots += 1;
        }
        else {
            render_memory_fetch(ppu);
            if (ppu->dots == 340) {
                if (ppu->frames % 2 == 1) { // odd frames
                    // we are jumping from (339, 261) to (0, 0) but we are still accessing the nametable byte, 
                    // so we implement this by skipping the (0, 0) to (1, 0) instead of skipping (340, 261). 
                    ppu->dots = 1; 
                    ppu->lines = 0;
                } else {
                    ppu->dots = 0;
                    ppu->lines = 0;
                }
                ppu->frames += 1;
                ppu->state = PPUSTATE_RENDER;
            } else {
                ppu->dots += 1;
            }
        }
    }
}

