#include "bus.h"

void bus_init(struct nesbus *bus) {
    memset(bus->cpu_ram, 0, 2048);
    bus->cycles_elapsed = 0; 
    bus->rom = NULL;
}

/// https://www.nesdev.org/wiki/CPU_memory_map
uint8_t bus_read8(struct nesbus *bus, uint16_t addr) {
    if (addr < 0x2000) {
        return bus->cpu_ram[addr % 0x0800]; // note: 0x0800 = 2048
    } else if (addr < 0x4000) {
        addr = 0x2000 + ((addr - 0x2000) % 8);
        // ppu_read8
        return 0xff;
    } else if (addr < 0x4018) { // APU registers and IO registers 
        return 0xff;
    } else { // Cartridge space 
        return (*(bus->PRG_reader))(bus->rom, addr); 
    }
}

void bus_write8(struct nesbus *bus, uint16_t addr, uint8_t value) {
    if (addr < 0x2000) {
        bus->cpu_ram[addr % 0x0800] = value; 
    } else if (addr < 0x4000) {
        addr = 0x2000 + ((addr - 0x2000) % 8);
        // ppu_write8
    } else if (addr < 0x4018) { // APU registers and IO registers 
    } else { // Cartridge space 
        (*(bus->PRG_writer))(bus->rom, addr, value); 
    }
}

uint8_t bus_peek8(struct nesbus *bus, uint16_t addr) {
    if (addr < 0x2000) {
        return bus->cpu_ram[addr % 0x0800]; 
    } else if (addr < 0x4000) {
        addr = 0x2000 + ((addr - 0x2000) % 8);
        // ppu_peek8
        return 0xff;
    } else if (addr < 0x4018) { // APU registers and IO registers 
        return 0xff;
    } else { // Cartridge space 
        return (*(bus->PRG_reader))(bus->rom, addr); 
    }
}

void bus_catchup_cycles(struct nesbus *bus, unsigned cycles) {
    /// TODO: tick the PPU and APU 
}


