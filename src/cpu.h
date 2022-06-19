/**
 * The NES's 6502 CPU 
 * Very useful information: 
 * Opcode description: https://llx.com/Neil/a2/opcodes.html 
 * The 6502 instruction set: https://www.masswerk.at/6502/6502_instruction_set.html 
 * The illegal opcodes: https://www.masswerk.at/nowgobang/2021/6502-illegal-opcodes 
 * The opcode table: https://www.nesdev.org/wiki/CPU_unofficial_opcodes 
 * The 6502 simulator: http://www.visual6502.org/JSSim/expert.html 
 * The instructions: http://www.6502.org/tutorials/6502opcodes.html 
 * Another opcode table: http://www.oxyron.de/html/opcodes02.html
 * 
 * How the flags are handled, especially the B and _ flags: https://www.nesdev.org/wiki/Status_flags#The_B_flag
 */
#pragma once 

#include "sdk.h"
#include "bus.h"
struct nescpu {
    uint8_t a; 
    uint8_t x; 
    uint8_t y; 
    uint8_t sp; 
    uint8_t p; // note: there's no break flags in the P register (bit 4 and bit 5)
    uint16_t pc; 

    struct nesbus *bus;
};

enum cpu_flags {
    CPU_FLAG_C /*cpu_flag_carry*/             = 0b00000001,
    CPU_FLAG_Z /*cpu_flag_zero*/              = 0b00000010,
    CPU_FLAG_I /*cpu_flag_interrupt_disable*/ = 0b00000100,
    CPU_FLAG_D /*cpu_flag_decimal_mode*/      = 0b00001000, // not supported 
    CPU_FLAG_B /*cpu_flag_break*/             = 0b00010000,
    CPU_FLAG__ /*cpu_flag_ignored*/           = 0b00100000, // usually ignored 
    CPU_FLAG_V /*cpu_flag_overflow*/          = 0b01000000, 
    CPU_FLAG_N /*cpu_flag_negative*/          = 0b10000000 
};


#define CPU_POWERUP_SP UINT8_C(0xFD)
#define CPU_IRQ_VECTOR UINT16_C(0xFFFE)
#define CPU_RESET_VECTOR UINT16_C(0xFFFC)
#define CPU_NMI_VECTOR UINT16_C(0xFFFA)
#define CPU_STACK_OFFSET UINT16_C(0x0100) 

void cpu_init(struct nescpu *cpu, struct nesbus *bus); 
void cpu_copy(struct nescpu *to_cpu, const struct nescpu *cpu); 
void cpu_reset(struct nescpu *cpu); 
int cpu_single_step(struct nescpu *cpu); 

void cpu_run(struct nescpu *cpu, void(*callback)(struct nescpu *c)); 


uint16_t cpu_compute_address(struct nescpu *cpu, uint8_t addrmode, uint16_t ptr, bool *is_page_crossed);


/// NOTE: the correct way to use inline in C 
/// is to define inline function in header file
/// then extern the inline functions in the C files! 
/// because we need to provide external linkage for the inline functions 
/// as the compiler may not choose to inline them 
/// this is not the case in C++ (!!!) 
/// in C++, purely define the inline functions in the header file is enough
/// as long as the definitions are identical in different compilation units
/// and the linker will "randomly" pick one to link
/// NOTE: in C, the static variables defined within an inlined function
/// are different between the inline definition and the external linkage



/// non-destructively reads a byte from the address addr (are the reads destructive?)
inline uint8_t cpu_peek8(struct nescpu *cpu, uint16_t addr) {
    return bus_peek8(cpu->bus, addr); 
}
/// reads a byte from the address addr 
inline uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr) {
    return bus_read8(cpu->bus, addr); 
}
inline void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value) {
    bus_write8(cpu->bus, addr, value); 
}

inline uint16_t cpu_peek16(struct nescpu *cpu, uint16_t addr) {
    return (
        ((uint16_t)cpu_peek8(cpu, addr)) | 
        (((uint16_t)cpu_peek8(cpu, addr+1)) << 8)
    );
}
inline uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr) {
    return (
        ((uint16_t)cpu_read8(cpu, addr)) | 
        (((uint16_t)cpu_read8(cpu, addr+1)) << 8)
    );
}
inline void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value) {
    cpu_write8(cpu, addr,   (uint8_t)(value & 0xFF));
    cpu_write8(cpu, addr+1, (uint8_t)(value >> 8)); 
}

#define OPCODE_CASES(C) \
C(0x00) C(0x01) C(0x02) C(0x03) C(0x04) C(0x05) C(0x06) C(0x07) \
C(0x08) C(0x09) C(0x0A) C(0x0B) C(0x0C) C(0x0D) C(0x0E) C(0x0F) \
C(0x10) C(0x11) C(0x12) C(0x13) C(0x14) C(0x15) C(0x16) C(0x17) \
C(0x18) C(0x19) C(0x1A) C(0x1B) C(0x1C) C(0x1D) C(0x1E) C(0x1F) \
C(0x20) C(0x21) C(0x22) C(0x23) C(0x24) C(0x25) C(0x26) C(0x27) \
C(0x28) C(0x29) C(0x2A) C(0x2B) C(0x2C) C(0x2D) C(0x2E) C(0x2F) \
C(0x30) C(0x31) C(0x32) C(0x33) C(0x34) C(0x35) C(0x36) C(0x37) \
C(0x38) C(0x39) C(0x3A) C(0x3B) C(0x3C) C(0x3D) C(0x3E) C(0x3F) \
C(0x40) C(0x41) C(0x42) C(0x43) C(0x44) C(0x45) C(0x46) C(0x47) \
C(0x48) C(0x49) C(0x4A) C(0x4B) C(0x4C) C(0x4D) C(0x4E) C(0x4F) \
C(0x50) C(0x51) C(0x52) C(0x53) C(0x54) C(0x55) C(0x56) C(0x57) \
C(0x58) C(0x59) C(0x5A) C(0x5B) C(0x5C) C(0x5D) C(0x5E) C(0x5F) \
C(0x60) C(0x61) C(0x62) C(0x63) C(0x64) C(0x65) C(0x66) C(0x67) \
C(0x68) C(0x69) C(0x6A) C(0x6B) C(0x6C) C(0x6D) C(0x6E) C(0x6F) \
C(0x70) C(0x71) C(0x72) C(0x73) C(0x74) C(0x75) C(0x76) C(0x77) \
C(0x78) C(0x79) C(0x7A) C(0x7B) C(0x7C) C(0x7D) C(0x7E) C(0x7F) \
C(0x80) C(0x81) C(0x82) C(0x83) C(0x84) C(0x85) C(0x86) C(0x87) \
C(0x88) C(0x89) C(0x8A) C(0x8B) C(0x8C) C(0x8D) C(0x8E) C(0x8F) \
C(0x90) C(0x91) C(0x92) C(0x93) C(0x94) C(0x95) C(0x96) C(0x97) \
C(0x98) C(0x99) C(0x9A) C(0x9B) C(0x9C) C(0x9D) C(0x9E) C(0x9F) \
C(0xA0) C(0xA1) C(0xA2) C(0xA3) C(0xA4) C(0xA5) C(0xA6) C(0xA7) \
C(0xA8) C(0xA9) C(0xAA) C(0xAB) C(0xAC) C(0xAD) C(0xAE) C(0xAF) \
C(0xB0) C(0xB1) C(0xB2) C(0xB3) C(0xB4) C(0xB5) C(0xB6) C(0xB7) \
C(0xB8) C(0xB9) C(0xBA) C(0xBB) C(0xBC) C(0xBD) C(0xBE) C(0xBF) \
C(0xC0) C(0xC1) C(0xC2) C(0xC3) C(0xC4) C(0xC5) C(0xC6) C(0xC7) \
C(0xC8) C(0xC9) C(0xCA) C(0xCB) C(0xCC) C(0xCD) C(0xCE) C(0xCF) \
C(0xD0) C(0xD1) C(0xD2) C(0xD3) C(0xD4) C(0xD5) C(0xD6) C(0xD7) \
C(0xD8) C(0xD9) C(0xDA) C(0xDB) C(0xDC) C(0xDD) C(0xDE) C(0xDF) \
C(0xE0) C(0xE1) C(0xE2) C(0xE3) C(0xE4) C(0xE5) C(0xE6) C(0xE7) \
C(0xE8) C(0xE9) C(0xEA) C(0xEB) C(0xEC) C(0xED) C(0xEE) C(0xEF) \
C(0xF0) C(0xF1) C(0xF2) C(0xF3) C(0xF4) C(0xF5) C(0xF6) C(0xF7) \
C(0xF8) C(0xF9) C(0xFA) C(0xFB) C(0xFC) C(0xFD) C(0xFE) C(0xFF) 

// extern const char opcode_nametable[256][4]; 
// void print_cpu_opcode_nametable(void);
void dump_cpu(struct nescpu *cpu);


