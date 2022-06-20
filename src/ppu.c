#include "ppu.h"
#include "cpu.h"
/// https://www.nesdev.org/wiki/PPU_power_up_state
void ppu_init(struct nesppu *ppu) {
    ppu->ctrl = 0;
    ppu->mask = 0;
    ppu->status = 0b10100000;
    ppu->oamaddr = 0;
    ppu->scroll_x = 0;
    ppu->scroll_y = 0;
    ppu->scroll_latching_y = false;
    ppu->addr = 0;
    ppu->addr_latching_lsb = false;
    ppu->data_read_buffer = 0;

    memset(ppu->oamdata, 0, 256);

    ppu->mirroring = MIRROR_HORI;

    ppu->CHR_reader = NULL;
    ppu->CHR_writer = NULL; 
    ppu->rom = NULL;

    ppu->iobus_last_value = 0;
    ppu->vmembus_last_value = 0;
}
void ppu_reset(struct nesppu *ppu) {
    ppu->ctrl = 0;
    ppu->mask = 0;
    ppu->scroll_x = 0;
    ppu->scroll_y = 0;
    ppu->scroll_latching_y = false;
    ppu->addr_latching_lsb = false;
    ppu->data_read_buffer = 0;
}
#define PANIC_ON_OPEN_BUS_READ 1
#if PANIC_ON_OPEN_BUS_READ
#define OPEN_BUS_READ(reg) do { panic(reg " is an open bus read\n"); return 0xFF; } while (0)
#else 
#define OPEN_BUS_READ(reg) return 0
#endif 
uint8_t ppu_external_read8(struct nesppu *ppu, uint16_t addr) {
    switch (addr) {
        case 0x2000: OPEN_BUS_READ("ppu control"); /// PPUCTRL
        case 0x2001: OPEN_BUS_READ("ppu mask"); /// PPUMASK
        case 0x2002: /// PPUSTATUS
        {   /// some quirks 
        /// see: https://www.nesdev.org/wiki/PPU_registers Status ($2002) < read
            uint8_t stat = ppu->status;
            ppu->status = set_mask(ppu->status, PPUSTATUS_VBLANK_STARTED, false);
            ppu->addr_latching_lsb = false;
            ppu->scroll_latching_y = false;
            return stat;
        }
        case 0x2003: OPEN_BUS_READ("ppu OAMADDR"); /// OAMADDR
        case 0x2004: /// OAMDATA
            return ppu->oamdata[ppu->oamaddr];
        case 0x2005: OPEN_BUS_READ("ppu scroll"); /// PPUSCROLL
        case 0x2006: OPEN_BUS_READ("ppu addr"); /// PPUADDR 
        case 0x2007: /// PPUDATA
            return ppu_internal_read8(ppu, ppu->addr); 
        case 0x4014: OPEN_BUS_READ("ppu OAMDMA"); /// OAMDMA
        default: 
            panic("address %04X does not belong to PPU\n", addr);
            return 0xFF;
    }
}
/// https://www.nesdev.org/wiki/PPU_registers
void ppu_external_write8(struct nesppu *ppu, uint16_t addr, uint8_t value) {
    switch (addr) {
        case 0x2000: /// PPUCTRL
            /// TODO: this is not exactly right, there are more stuffs to do 
            ppu->ctrl = value;
            break;
        case 0x2001: /// PPUMASK
            ppu->mask = value;
            break;
        case 0x2002: /// PPUSTATUS
            panic("ppu status is not writable\n");
            break;
        case 0x2003: /// OAMADDR
            ppu->oamaddr = value;
            break;
        case 0x2004: /// OAMDATA
            /// the oam address is incremented by 1 automatically
            ppu->oamdata[ppu->oamaddr] = value;
            ppu->oamaddr += 1;
            break;
        case 0x2005: /// PPUSCROLL
            if (ppu->scroll_latching_y) ppu->scroll_y = value;
            else ppu->scroll_x = value;
            ppu->scroll_latching_y = !(ppu->scroll_latching_y);
            break;
        case 0x2006: /// PPUADDR 
            if (ppu->addr_latching_lsb) { ppu->addr &= 0xFF00; ppu->addr |= (uint16_t)value; } 
            else { ppu->addr &= 0x00FF; ppu->addr |= (((uint16_t)value) << 8); }
            ppu->addr &= 0x3FFF;
            ppu->addr_latching_lsb = !(ppu->addr_latching_lsb);
            break;
        case 0x2007: /// PPUDATA
            ppu_internal_write8(ppu, ppu->addr, value);
            break;
        case 0x4014: /// OAMDMA
            /// TODO: implement this 
            /// The CPU is suspended during the DMA 
            /// and it only copies a page to the oam memory
            /// take 513 or 514 cycles after the $4014 write tick. 
            /// (1 wait state cycle while waiting for writes to complete, 
            /// +1 if on an odd CPU cycle, 
            /// then 256 alternating read/write cycles.)
            break;
        default: 
            panic("address %04X does not belong to PPU\n", addr);
            break;
    }
}

uint8_t ppu_external_peek8(struct nesppu *ppu, uint16_t addr) {
    switch (addr) {
        case 0x2000: /// PPUCTRL
            return ppu->ctrl;
        case 0x2001: /// PPUMASK
            return ppu->mask;
        case 0x2002: /// PPUSTATUS
            return ppu->status;
        case 0x2003: /// OAMADDR
            return ppu->oamaddr;
        case 0x2004: /// OAMDATA
            return ppu->oamdata[ppu->oamaddr];
        case 0x2005: /// PPUSCROLL
            return ppu->scroll_latching_y ? ppu->scroll_y : ppu->scroll_x;
        case 0x2006: /// PPUADDR 
            return ppu->addr_latching_lsb ? ppu->addr : ppu->addr >> 8;
        case 0x2007: /// PPUDATA
            return 0;
        case 0x4014: /// OAMDMA
            return 0;
        default: panic("address %04X does not belong to PPU\n", addr);
            return 0xFF;
    }
}



uint8_t ppu_internal_read8(struct nesppu *ppu, uint16_t addr) {
    return 0xFF;
}
void ppu_internal_write8(struct nesppu *ppu, uint16_t addr, uint8_t value) {

}








