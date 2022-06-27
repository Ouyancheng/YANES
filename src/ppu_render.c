#include "ppu.h"
#include "cpu.h"

/** 
 * a tick that does nothing but increment the dots and possibly the lines 
 * (maybe) better not to inline it because we may want less branches to relief the branch predictor 
 */
static void idle_tick(struct nesppu *ppu) {
    if (ppu->dots <= 340) {
        ppu->dots += 1;
    } else {
        ppu->dots = 0;
        ppu->lines += 1;
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
static inline void coarse_X_increment(struct nesppu *ppu) {
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
static inline void y_increment(struct nesppu *ppu) {
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
/** tick the ppu by one ppu cycle, or by num_cycles? */
void ppu_tick(struct nesppu *ppu, unsigned num_cycles) {
    ppu->cycles += num_cycles;
    if (ppu->lines < 240) { // rendering
        /** TODO: handle not rendering behaviour correctly */
        // if (!(ppu->mask & PPUMASK_SHOW_BACKGROUND) && !(ppu->mask & PPUMASK_SHOW_SPRITES)) {
        //     idle_tick(ppu);
        //     return;
        // }
        if (ppu->dots == 0) {
            // render? 
            ppu->dots += 1;
        }
        else if (ppu->dots <= 256) {
            unsigned dots_m_1 = ppu->dots - 1;
            switch (dots_m_1 % 8) {
                case 1: {
                    // fetch nametable byte
                    uint8_t nametable_byte = fetch_nametable_byte(ppu);
                    break;
                }
                case 3: {
                    // fetch attribute table byte
                    uint8_t attribute_table_byte = fetch_attribute_byte(ppu);

                    break;
                }
                    
                case 5: 
                    // fetch lower background tile byte
                    break;
                case 7: 
                    // fetch higher background tile byte and increment the horizontal v
                    
                    break;
                default:
                    break;
            }
            ppu->dots += 1;
        }
        else if (ppu->dots == 257) {

        }
        else if (ppu->dots <= 320) {

        }
        else {

        }
    }
    else if (ppu->lines < 260) { // vblank + // post render
        idle_tick(ppu);
        // set the vblank on the second tick otherwise idle
        if (ppu->lines == 241 && ppu->dots == 1) { ppu->nmi_raised = true; }
    }
    else { // pre-render

    }
}

