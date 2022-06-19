#include "mapper0.h"


uint8_t NROM_read_PRG(struct nesrom *rom, uint16_t addr) {
    addr -= 0x8000;
    if (rom->PRG_rom_size == 0x4000 && addr >= 0x4000) {
        addr %= 0x4000;
    }
    return *(rom->rom_data + rom->PRG_rom_offset + addr);
}
void NROM_write_PRG(struct nesrom *rom, uint16_t addr, uint8_t value) {
    panic("writing to PRG rom\n");
}
uint8_t NROM_read_CHR(struct nesrom *rom, uint16_t addr) {
    return *(rom->rom_data + rom->PRG_rom_offset + rom->PRG_rom_size + addr);
}
void NROM_write_CHR(struct nesrom *rom, uint16_t addr, uint8_t value) {
    panic("writing to CHR rom\n");
}

