#include "ppu.h"
#include "cpu.h"

/** https://www.nesdev.org/wiki/PPU_power_up_state */
void ppu_init(struct nesppu *ppu) {
    ppu->v = 0;
    ppu->t = 0;
    ppu->x = 0;
    ppu->w = 0;
    ppu->ctrl = 0;
    ppu->mask = 0;
    ppu->status = 0b10100000;
    ppu->oamaddr = 0;
    ppu->data_read_buffer = 0;
    ppu_set_nametable_mirror(ppu, MIRROR_VERT);
    ppu->rom = NULL;
    ppu->CHR_reader = NULL;
    ppu->CHR_writer = NULL;
    ppu->iobus_last_value = 0;
    ppu->nmi_raised = false;
    ppu->state = PPUSTATE_RENDER;
    ppu->frames = 0;
    ppu->cycles = 0;
    ppu->dots = 0;
    ppu->lines = 0;
    memset(ppu->palette_ram, 0, 32);
    memset(ppu->oamdata, 0, 256); 
    memset(ppu->vram, 0, 2048);
}

void ppu_reset(struct nesppu *ppu) {
    ppu->v &= 0x0FFF; // the PPUADDR are unchanged, but PPU scroll is zeroed?
    ppu->t &= 0x0FFF;
    ppu->x = 0;
    ppu->w = 0;
    ppu->data_read_buffer = 0;
    ppu->ctrl = 0;
    ppu->mask = 0;
    ppu->state = PPUSTATE_RENDER;
    ppu->cycles = 7 * 3; // because cpu handling the reset interrupt takes 7 cycles
    ppu->dots = 0;
    ppu->lines = 0;
    // odd frame is set to false 
    ppu->frames = 0; 
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
/** Be super careful on implementing this: https://www.nesdev.org/wiki/PPU_registers */
uint8_t ppu_external_read8(struct nesppu *ppu, uint16_t addr) {
    uint8_t value;
    switch (addr) {
        case PPUCTRL: return ppu->iobus_last_value;
        case PPUMASK: return ppu->iobus_last_value;
        case PPUSTATUS: {
            uint8_t stat = ppu->status;
            ppu->status &= (~PPUSTATUS_VBLANK_STARTED);
            ppu->w = 0;
            value = (stat | (ppu->iobus_last_value & 0b00011111));
            break;
        }
        case OAMADDR: return ppu->iobus_last_value;
        case OAMDATA: 
            value = ppu->oamdata[ppu->oamaddr];
            break;
        case PPUSCROLL: return ppu->iobus_last_value;
        case PPUADDR: return ppu->iobus_last_value;
        case PPUDATA: {
            value = ppu_internal_read8(ppu, ppu->v & 0x3FFF);
            ppu->v += ((ppu->ctrl & PPUCTRL_VRAM_ADDR_INCREMENT) ? 32 : 1);
            /*
            TODO: 
            During rendering (on the pre-render line and the visible lines 0-239, 
            provided either background or sprite rendering is enabled), 
            it will update v in an odd way, 
            triggering a coarse X increment and a Y increment simultaneously (with normal wrapping behavior).
            */
            break;
        }
        default: {
            panic("address %04X is not a PPU address\n", addr);
        }
    }
    ppu->iobus_last_value = value;
    return value;
}

void ppu_external_write8(struct nesppu *ppu, uint16_t addr, uint8_t value) {
    ppu->iobus_last_value = value;
    switch (addr) {
        case PPUCTRL: {
            ppu->t &= UINT16_C(0b1111001111111111);
            ppu->t |= ((uint16_t)value & UINT16_C(0b11)) << 10;
            bool prev_nmi_enabled = (ppu->ctrl & PPUCTRL_GENERATE_NMI);
            bool vblank_flag_is_set = (ppu->status & PPUSTATUS_VBLANK_STARTED);
            ppu->ctrl = value;
            if (vblank_flag_is_set && !prev_nmi_enabled && (value & PPUCTRL_GENERATE_NMI)) {
                ppu->nmi_raised = true;
            }
            /** TODO: there's a bit 0 race condition, but let's leave it away... */
            break;
        }
        case PPUMASK: ppu->mask = value; break;
        case PPUSTATUS: break;
        case OAMADDR: ppu->oamaddr = value; break;
        case OAMDATA: {
            /** TODO: For emulation purposes, it is probably best to completely ignore writes during rendering. */
            ppu->oamdata[ppu->oamaddr++] = value; 
            break;
        }
        case PPUSCROLL: {
            uint16_t d = (uint16_t)value;
            if (ppu->w == 0) {
                ppu->t &= (~UINT16_C(0b11111));
                ppu->t |= ((d >> 3) & 0b11111);
                ppu->x = (uint8_t)(d & 0b111);
                ppu->w = 1;
            } else {
                ppu->t &= UINT16_C(0b000110000011111);
                ppu->t |= ((d >> 3) << 5);
                ppu->t |= ((d & 0b111) << 12);
                ppu->w = 0;
            }
            break;
        }
        case PPUADDR: {
            uint16_t d = (uint16_t)value;
            if (ppu->w == 0) {
                ppu->t &= UINT16_C(0b000000011111111);
                ppu->t |= ((d & 0b111111) << 8);
                ppu->w = 1;
            } else {
                ppu->t &= UINT16_C(0xFF00);
                ppu->t |= (d & 0xFF);
                ppu->v = ppu->t;
                ppu->w = 0;
            }
            break;
        }
        case PPUDATA: {
            ppu_internal_write8(ppu, ppu->v & 0x3FFF, value);
            ppu->v += ((ppu->ctrl & PPUCTRL_VRAM_ADDR_INCREMENT) ? 32 : 1);
            /*
            TODO: (also in the read counterpart)
            During rendering (on the pre-render line and the visible lines 0-239, 
            provided either background or sprite rendering is enabled), 
            it will update v in an odd way, 
            triggering a coarse X increment and a Y increment simultaneously (with normal wrapping behavior).
            */
            break;
        }
        default: {
            panic("address %04X is not a PPU address\n", addr);
        }
    }
}

uint8_t ppu_external_peek8(struct nesppu *ppu, uint16_t addr) {
    switch (addr) {
        case PPUCTRL: return ppu->ctrl;
        case PPUMASK: return ppu->mask;
        case PPUSTATUS: return ppu->status;
        case OAMADDR: return ppu->oamaddr;
        case OAMDATA: return ppu->oamdata[ppu->oamaddr];
        case PPUSCROLL: return ppu->t;
        case PPUADDR: return ppu->v;
        case PPUDATA: return 0;
        default: {
            panic("address %04X is not a PPU address\n", addr);
            return 0;
        }
    }
}
/** https://www.nesdev.org/wiki/PPU_memory_map */
uint8_t ppu_internal_read8(struct nesppu *ppu, uint16_t addr) {
    addr &= 0x3FFF;
    if (addr < 0x2000) {
        uint8_t result = ppu->data_read_buffer;
        ppu->data_read_buffer = ppu->CHR_reader(ppu, ppu->rom, addr);
        return result;
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
        /* 
        Reading the palettes still updates the internal buffer though, 
        but the data placed in it is the mirrored nametable data that would appear "underneath" the palette.
        */ 
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
/** https://www.nesdev.org/wiki/PPU_registers#OAM_DMA_($4014)_%3E_write */
void ppu_oam_dma(struct nesppu *ppu, struct nescpu *cpu, uint8_t page) {
    uint16_t hi = ((uint16_t)page) << 8; 
    unsigned cyc = ((cpu->cycles % 2 == 1) ? 2 : 1); // ((cpu->cycles % 2 == 1) ? 514 : 513);
    cpu->cycles += cyc;
    for (unsigned i = 0; i < 256; ++i) {
        ppu->oamdata[ppu->oamaddr] = cpu_read8(cpu, hi + i); 
        ppu->oamaddr += 1; 
        ppu->oamaddr %= 256;
        cpu->cycles += 2; 
    }
}

