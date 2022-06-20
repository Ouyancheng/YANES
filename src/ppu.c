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
    memset(ppu->palette_ram, 0, 64);
    memset(ppu->oamdata, 0, 256);
    memset(ppu->vram, 0, 2048);

    ppu->mirroring = MIRROR_HORI;
    ppu->nametables[0] = (uint8_t*)(ppu->vram);
    ppu->nametables[1] = (uint8_t*)(ppu->vram);
    ppu->nametables[2] = (uint8_t*)(ppu->vram) + 1024;
    ppu->nametables[3] = (uint8_t*)(ppu->vram) + 1024;

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
void ppu_set_nametable_mirror(struct nesppu *ppu, enum nametable_mirror mirroring) {
    switch (mirroring) {
        case MIRROR_FOUR:
        case MIRROR_VERT:
            ppu->nametables[0] = (uint8_t*)(ppu->vram);
            ppu->nametables[1] = (uint8_t*)(ppu->vram) + 1024;
            ppu->nametables[2] = (uint8_t*)(ppu->vram);
            ppu->nametables[3] = (uint8_t*)(ppu->vram) + 1024;
            ppu->mirroring = mirroring;
            break;
        case MIRROR_HORI:
            ppu->nametables[0] = (uint8_t*)(ppu->vram);
            ppu->nametables[1] = (uint8_t*)(ppu->vram);
            ppu->nametables[2] = (uint8_t*)(ppu->vram) + 1024;
            ppu->nametables[3] = (uint8_t*)(ppu->vram) + 1024;
            ppu->mirroring = mirroring;
            break;
        default: 
            printf("unknown mirroring %u\n", (unsigned)mirroring);
            break;
    }
}
#define PANIC_ON_OPEN_BUS_READ 0
#if PANIC_ON_OPEN_BUS_READ
#define OPEN_IOBUS_READ(reg) do { panic(reg " is an open bus read\n"); return 0xFF; } while (0)
#else 
#define OPEN_IOBUS_READ(reg) return ppu->iobus_last_value
#endif 
/// see: https://www.nesdev.org/wiki/Open_bus_behavior#PPU_open_bus for how to handle open bus reads
uint8_t ppu_external_read8(struct nesppu *ppu, uint16_t addr) {
    uint8_t value;
    switch (addr) {
        case 0x2000: OPEN_IOBUS_READ("ppu control"); /// PPUCTRL
        case 0x2001: OPEN_IOBUS_READ("ppu mask"); /// PPUMASK
        case 0x2002: /// PPUSTATUS
        {   /// some quirks 
        /// see: https://www.nesdev.org/wiki/PPU_registers Status ($2002) < read
            uint8_t stat = ppu->status;
            ppu->status = set_mask(ppu->status, PPUSTATUS_VBLANK_STARTED, false);
            ppu->addr_latching_lsb = false;
            ppu->scroll_latching_y = false;
            value = (stat | (ppu->iobus_last_value & 0b00011111)); // see the open bus read
            break;
        }
        case 0x2003: OPEN_IOBUS_READ("ppu OAMADDR"); /// OAMADDR
        case 0x2004: /// OAMDATA
            value = ppu->oamdata[ppu->oamaddr];
            break;
        case 0x2005: OPEN_IOBUS_READ("ppu scroll"); /// PPUSCROLL
        case 0x2006: OPEN_IOBUS_READ("ppu addr"); /// PPUADDR 
        case 0x2007: /// PPUDATA
            value = ppu_internal_read8(ppu, ppu->addr); 
            /// NOTE:: Increment PPU's internal address!
            ppu->addr += ((ppu->ctrl & PPUCTRL_VRAM_ADDR_INCREMENT) ? 32 : 1);
            ppu->addr &= 0x3FFF;
            break;
        // case 0x4014: OPEN_IOBUS_READ("ppu OAMDMA"); /// OAMDMA
        default: 
            panic("address %04X does not belong to PPU\n", addr);
            return 0xFF;
    }
    ppu->iobus_last_value = value;
    return value;
}
/// https://www.nesdev.org/wiki/PPU_registers
void ppu_external_write8(struct nesppu *ppu, uint16_t addr, uint8_t value) {
    ppu->iobus_last_value = value;
    switch (addr) {
        case 0x2000: /// PPUCTRL
            /// TODO: this is not exactly right, there are more stuffs to do 
            ppu->ctrl = value;
            break;
        case 0x2001: /// PPUMASK
            ppu->mask = value;
            break;
        case 0x2002: /// PPUSTATUS
            // panic("ppu status is not writable\n");
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
            /// NOTE: Increment PPU's internal address!
            ppu->addr += ((ppu->ctrl & PPUCTRL_VRAM_ADDR_INCREMENT) ? 32 : 1);
            ppu->addr &= 0x3FFF;
            break;
        // case 0x4014: /// OAMDMA
            /// TODO: implement this 
            /// The CPU is suspended during the DMA 
            /// and it only copies a page to the oam memory
            /// take 513 or 514 cycles after the $4014 write tick. 
            /// (1 wait state cycle while waiting for writes to complete, 
            /// +1 if on an odd CPU cycle, 
            /// then 256 alternating read/write cycles.)
            // break;
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
        // case 0x4014: /// OAMDMA
        //     return 0;
        default: panic("address %04X does not belong to PPU\n", addr);
            return 0xFF;
    }
}


#if PANIC_ON_OPEN_BUS_READ
#define OPEN_VMEMBUS_READ(reg) do { panic(reg " is an open bus read\n"); return 0xFF; } while (0)
#else 
#define OPEN_VMEMBUS_READ(reg) return (addr & 0xFF)
#endif 
/// see this for the open bus behaviour https://www.nesdev.org/wiki/Open_bus_behavior#PPU_open_bus
uint8_t ppu_internal_read8(struct nesppu *ppu, uint16_t addr) {
    addr &= 0x3FFF;
    /// TODO: implement this
    if (addr < 0x2000) {
        uint8_t result = ppu->data_read_buffer;
        ppu->data_read_buffer = ppu->CHR_reader(ppu, ppu->rom, addr);
    } 
    else if (addr < 0x3F00) {
        addr &= UINT16_C(0x2fff);
        uint8_t result = ppu->data_read_buffer;
        unsigned nametable_index = (addr - 0x2000) / 0x0400;
        ppu->data_read_buffer = ppu->nametables[nametable_index][addr - (0x2000 + 0x0400 * nametable_index)];
        return result;
    }
    else {
        uint8_t result;
        /// Reading the palettes still updates the internal buffer though, 
        /// but the data placed in it is the mirrored nametable data that would appear "underneath" the palette.
        uint16_t nametable_mirror_addr = addr & UINT16_C(0x2fff);
        ppu->data_read_buffer = ppu->nametables[3][nametable_mirror_addr - 0x2C00];
        uint16_t palette_addr = (addr - UINT16_C(0x3f00)) % UINT16_C(32); // addr = 0x3f00 - 0x3f1f 
        if (palette_addr == 0x10 || palette_addr == 0x14 || palette_addr == 0x18 || palette_addr == 0x1c) {
            result = ppu->palette_ram[palette_addr-0x10]; 
        } else {
            result = ppu->palette_ram[palette_addr]; 
        }
        return result;
    }
}
void ppu_internal_write8(struct nesppu *ppu, uint16_t addr, uint8_t value) {
    /// TODO: implement this
    addr &= 0x3FFF;
    if (addr < 0x2000) {
        ppu->CHR_writer(ppu, ppu->rom, addr, value);
    } 
    else if (addr < 0x3F00) {
        addr &= UINT16_C(0x2fff);
        unsigned nametable_index = (addr - 0x2000) / 0x0400;
        ppu->nametables[nametable_index][addr - (0x2000 + 0x0400 * nametable_index)] = value;
    }
    else {
        uint16_t palette_addr = (addr - UINT16_C(0x3f00)) % UINT16_C(32); // addr = 0x3f00 - 0x3f1f 
        if (palette_addr == 0x10 || palette_addr == 0x14 || palette_addr == 0x18 || palette_addr == 0x1c) {
            ppu->palette_ram[palette_addr-0x10] = value; 
        } else {
            ppu->palette_ram[palette_addr] = value; 
        }
    }
}








