#pragma once 
#include "sdk.h"
#include "macros.h"
enum addressmode {
    MODE_ABS, // addressmode_absolute, 
    MODE_ZPG, // addressmode_zeropage, 
    MODE_ZPX, // addressmode_zeropage_x, 
    MODE_ZPY, // addressmode_zeropage_y, 
    MODE_ABX, // addressmode_absolute_x, 
    MODE_ABY, // addressmode_absolute_y, 
    MODE_IMM, // addressmode_immediate, 
    MODE_REL, // addressmode_relative, 
    MODE_IMP, // addressmode_implicit, 
    MODE_IND, // addressmode_indirect, 
    MODE_INX, // addressmode_indirect_x, 
    MODE_INY, // addressmode_indirect_y, 
    // MODE_ACC  // addressmode_accumulator
};



#define OP_MODE_0x00 MODE_IMP
#define OP_MODE_0x20 MODE_ABS
#define OP_MODE_0x40 MODE_IMP
#define OP_MODE_0x60 MODE_IMP
#define OP_MODE_0x80 MODE_IMM
#define OP_MODE_0xA0 MODE_IMM
#define OP_MODE_0xC0 MODE_IMM
#define OP_MODE_0xE0 MODE_IMM


#define OP_MODE_0x01 MODE_INX
#define OP_MODE_0x21 MODE_INX
#define OP_MODE_0x41 MODE_INX
#define OP_MODE_0x61 MODE_INX
#define OP_MODE_0x81 MODE_INX
#define OP_MODE_0xA1 MODE_INX
#define OP_MODE_0xC1 MODE_INX
#define OP_MODE_0xE1 MODE_INX

#define OP_MODE_0x02 MODE_IMP
#define OP_MODE_0x22 MODE_IMP
#define OP_MODE_0x42 MODE_IMP
#define OP_MODE_0x62 MODE_IMP
#define OP_MODE_0x82 MODE_IMM
#define OP_MODE_0xA2 MODE_IMM
#define OP_MODE_0xC2 MODE_IMM
#define OP_MODE_0xE2 MODE_IMM

#define OP_MODE_0x03 MODE_INX
#define OP_MODE_0x23 MODE_INX
#define OP_MODE_0x43 MODE_INX
#define OP_MODE_0x63 MODE_INX
#define OP_MODE_0x83 MODE_INX
#define OP_MODE_0xA3 MODE_INX
#define OP_MODE_0xC3 MODE_INX
#define OP_MODE_0xE3 MODE_INX


#define OP_MODE_0x04 MODE_ZPG
#define OP_MODE_0x24 MODE_ZPG
#define OP_MODE_0x44 MODE_ZPG
#define OP_MODE_0x64 MODE_ZPG
#define OP_MODE_0x84 MODE_ZPG
#define OP_MODE_0xA4 MODE_ZPG
#define OP_MODE_0xC4 MODE_ZPG
#define OP_MODE_0xE4 MODE_ZPG

#define OP_MODE_0x05 MODE_ZPG
#define OP_MODE_0x25 MODE_ZPG
#define OP_MODE_0x45 MODE_ZPG
#define OP_MODE_0x65 MODE_ZPG
#define OP_MODE_0x85 MODE_ZPG
#define OP_MODE_0xA5 MODE_ZPG
#define OP_MODE_0xC5 MODE_ZPG
#define OP_MODE_0xE5 MODE_ZPG

#define OP_MODE_0x06 MODE_ZPG
#define OP_MODE_0x26 MODE_ZPG
#define OP_MODE_0x46 MODE_ZPG
#define OP_MODE_0x66 MODE_ZPG
#define OP_MODE_0x86 MODE_ZPG
#define OP_MODE_0xA6 MODE_ZPG
#define OP_MODE_0xC6 MODE_ZPG
#define OP_MODE_0xE6 MODE_ZPG

#define OP_MODE_0x07 MODE_ZPG
#define OP_MODE_0x27 MODE_ZPG
#define OP_MODE_0x47 MODE_ZPG
#define OP_MODE_0x67 MODE_ZPG
#define OP_MODE_0x87 MODE_ZPG
#define OP_MODE_0xA7 MODE_ZPG
#define OP_MODE_0xC7 MODE_ZPG
#define OP_MODE_0xE7 MODE_ZPG

#define OP_MODE_0x08 MODE_IMP
#define OP_MODE_0x28 MODE_IMP
#define OP_MODE_0x48 MODE_IMP
#define OP_MODE_0x68 MODE_IMP
#define OP_MODE_0x88 MODE_IMP
#define OP_MODE_0xA8 MODE_IMP
#define OP_MODE_0xC8 MODE_IMP
#define OP_MODE_0xE8 MODE_IMP

#define OP_MODE_0x09 MODE_IMM
#define OP_MODE_0x29 MODE_IMM
#define OP_MODE_0x49 MODE_IMM
#define OP_MODE_0x69 MODE_IMM
#define OP_MODE_0x89 MODE_IMM
#define OP_MODE_0xA9 MODE_IMM
#define OP_MODE_0xC9 MODE_IMM
#define OP_MODE_0xE9 MODE_IMM


#define OP_MODE_0x0A MODE_IMP // MODE_ACC
#define OP_MODE_0x2A MODE_IMP // MODE_ACC
#define OP_MODE_0x4A MODE_IMP // MODE_ACC
#define OP_MODE_0x6A MODE_IMP // MODE_ACC
#define OP_MODE_0x8A MODE_IMP
#define OP_MODE_0xAA MODE_IMP
#define OP_MODE_0xCA MODE_IMP
#define OP_MODE_0xEA MODE_IMP

#define OP_MODE_0x0B MODE_IMM
#define OP_MODE_0x2B MODE_IMM
#define OP_MODE_0x4B MODE_IMM
#define OP_MODE_0x6B MODE_IMM
#define OP_MODE_0x8B MODE_IMM
#define OP_MODE_0xAB MODE_IMM
#define OP_MODE_0xCB MODE_IMM
#define OP_MODE_0xEB MODE_IMM

#define OP_MODE_0x0C MODE_ABS
#define OP_MODE_0x2C MODE_ABS
#define OP_MODE_0x4C MODE_ABS
#define OP_MODE_0x6C MODE_IND
#define OP_MODE_0x8C MODE_ABS
#define OP_MODE_0xAC MODE_ABS
#define OP_MODE_0xCC MODE_ABS
#define OP_MODE_0xEC MODE_ABS

#define OP_MODE_0x0D MODE_ABS
#define OP_MODE_0x2D MODE_ABS
#define OP_MODE_0x4D MODE_ABS
#define OP_MODE_0x6D MODE_ABS
#define OP_MODE_0x8D MODE_ABS
#define OP_MODE_0xAD MODE_ABS
#define OP_MODE_0xCD MODE_ABS
#define OP_MODE_0xED MODE_ABS

#define OP_MODE_0x0E MODE_ABS
#define OP_MODE_0x2E MODE_ABS
#define OP_MODE_0x4E MODE_ABS
#define OP_MODE_0x6E MODE_ABS
#define OP_MODE_0x8E MODE_ABS
#define OP_MODE_0xAE MODE_ABS
#define OP_MODE_0xCE MODE_ABS
#define OP_MODE_0xEE MODE_ABS

#define OP_MODE_0x0F MODE_ABS
#define OP_MODE_0x2F MODE_ABS
#define OP_MODE_0x4F MODE_ABS
#define OP_MODE_0x6F MODE_ABS
#define OP_MODE_0x8F MODE_ABS
#define OP_MODE_0xAF MODE_ABS
#define OP_MODE_0xCF MODE_ABS
#define OP_MODE_0xEF MODE_ABS

#define OP_MODE_0x10 MODE_REL
#define OP_MODE_0x30 MODE_REL
#define OP_MODE_0x50 MODE_REL
#define OP_MODE_0x70 MODE_REL
#define OP_MODE_0x90 MODE_REL
#define OP_MODE_0xB0 MODE_REL
#define OP_MODE_0xD0 MODE_REL
#define OP_MODE_0xF0 MODE_REL

#define OP_MODE_0x11 MODE_INY
#define OP_MODE_0x31 MODE_INY
#define OP_MODE_0x51 MODE_INY
#define OP_MODE_0x71 MODE_INY
#define OP_MODE_0x91 MODE_INY
#define OP_MODE_0xB1 MODE_INY
#define OP_MODE_0xD1 MODE_INY
#define OP_MODE_0xF1 MODE_INY

#define OP_MODE_0x12 MODE_IMP
#define OP_MODE_0x32 MODE_IMP
#define OP_MODE_0x52 MODE_IMP
#define OP_MODE_0x72 MODE_IMP
#define OP_MODE_0x92 MODE_IMP
#define OP_MODE_0xB2 MODE_IMP
#define OP_MODE_0xD2 MODE_IMP
#define OP_MODE_0xF2 MODE_IMP

#define OP_MODE_0x13 MODE_INY
#define OP_MODE_0x33 MODE_INY
#define OP_MODE_0x53 MODE_INY
#define OP_MODE_0x73 MODE_INY
#define OP_MODE_0x93 MODE_INY
#define OP_MODE_0xB3 MODE_INY
#define OP_MODE_0xD3 MODE_INY
#define OP_MODE_0xF3 MODE_INY

#define OP_MODE_0x14 MODE_ZPX
#define OP_MODE_0x34 MODE_ZPX
#define OP_MODE_0x54 MODE_ZPX
#define OP_MODE_0x74 MODE_ZPX
#define OP_MODE_0x94 MODE_ZPX
#define OP_MODE_0xB4 MODE_ZPX
#define OP_MODE_0xD4 MODE_ZPX
#define OP_MODE_0xF4 MODE_ZPX

#define OP_MODE_0x15 MODE_ZPX
#define OP_MODE_0x35 MODE_ZPX
#define OP_MODE_0x55 MODE_ZPX
#define OP_MODE_0x75 MODE_ZPX
#define OP_MODE_0x95 MODE_ZPX
#define OP_MODE_0xB5 MODE_ZPX
#define OP_MODE_0xD5 MODE_ZPX
#define OP_MODE_0xF5 MODE_ZPX

#define OP_MODE_0x16 MODE_ZPX
#define OP_MODE_0x36 MODE_ZPX
#define OP_MODE_0x56 MODE_ZPX
#define OP_MODE_0x76 MODE_ZPX
#define OP_MODE_0x96 MODE_ZPY
#define OP_MODE_0xB6 MODE_ZPY
#define OP_MODE_0xD6 MODE_ZPX
#define OP_MODE_0xF6 MODE_ZPX

#define OP_MODE_0x17 MODE_ZPX
#define OP_MODE_0x37 MODE_ZPX
#define OP_MODE_0x57 MODE_ZPX
#define OP_MODE_0x77 MODE_ZPX
#define OP_MODE_0x97 MODE_ZPY
#define OP_MODE_0xB7 MODE_ZPY
#define OP_MODE_0xD7 MODE_ZPX
#define OP_MODE_0xF7 MODE_ZPX

#define OP_MODE_0x18 MODE_IMP
#define OP_MODE_0x38 MODE_IMP
#define OP_MODE_0x58 MODE_IMP
#define OP_MODE_0x78 MODE_IMP
#define OP_MODE_0x98 MODE_IMP
#define OP_MODE_0xB8 MODE_IMP
#define OP_MODE_0xD8 MODE_IMP
#define OP_MODE_0xF8 MODE_IMP

#define OP_MODE_0x19 MODE_ABY
#define OP_MODE_0x39 MODE_ABY
#define OP_MODE_0x59 MODE_ABY
#define OP_MODE_0x79 MODE_ABY
#define OP_MODE_0x99 MODE_ABY
#define OP_MODE_0xB9 MODE_ABY
#define OP_MODE_0xD9 MODE_ABY
#define OP_MODE_0xF9 MODE_ABY

#define OP_MODE_0x1A MODE_IMP
#define OP_MODE_0x3A MODE_IMP
#define OP_MODE_0x5A MODE_IMP
#define OP_MODE_0x7A MODE_IMP
#define OP_MODE_0x9A MODE_IMP
#define OP_MODE_0xBA MODE_IMP
#define OP_MODE_0xDA MODE_IMP
#define OP_MODE_0xFA MODE_IMP

#define OP_MODE_0x1B MODE_ABY
#define OP_MODE_0x3B MODE_ABY
#define OP_MODE_0x5B MODE_ABY
#define OP_MODE_0x7B MODE_ABY
#define OP_MODE_0x9B MODE_ABY
#define OP_MODE_0xBB MODE_ABY
#define OP_MODE_0xDB MODE_ABY
#define OP_MODE_0xFB MODE_ABY

#define OP_MODE_0x1C MODE_ABX
#define OP_MODE_0x3C MODE_ABX
#define OP_MODE_0x5C MODE_ABX
#define OP_MODE_0x7C MODE_ABX
#define OP_MODE_0x9C MODE_ABX
#define OP_MODE_0xBC MODE_ABX
#define OP_MODE_0xDC MODE_ABX
#define OP_MODE_0xFC MODE_ABX

#define OP_MODE_0x1D MODE_ABX
#define OP_MODE_0x3D MODE_ABX
#define OP_MODE_0x5D MODE_ABX
#define OP_MODE_0x7D MODE_ABX
#define OP_MODE_0x9D MODE_ABX
#define OP_MODE_0xBD MODE_ABX
#define OP_MODE_0xDD MODE_ABX
#define OP_MODE_0xFD MODE_ABX

#define OP_MODE_0x1E MODE_ABX
#define OP_MODE_0x3E MODE_ABX
#define OP_MODE_0x5E MODE_ABX
#define OP_MODE_0x7E MODE_ABX
#define OP_MODE_0x9E MODE_ABY
#define OP_MODE_0xBE MODE_ABY
#define OP_MODE_0xDE MODE_ABX
#define OP_MODE_0xFE MODE_ABX

#define OP_MODE_0x1F MODE_ABX
#define OP_MODE_0x3F MODE_ABX
#define OP_MODE_0x5F MODE_ABX
#define OP_MODE_0x7F MODE_ABX
#define OP_MODE_0x9F MODE_ABY
#define OP_MODE_0xBF MODE_ABY
#define OP_MODE_0xDF MODE_ABX
#define OP_MODE_0xFF MODE_ABX

#define GETADDR_MODE_IMP(addr, cpu, ptr, page_crossed) ((void)0)
#define GETADDR_MODE_ACC(addr, cpu, ptr, page_crossed) ((void)0)
#define GETADDR_MODE_IMM(addr, cpu, ptr, page_crossed) do { addr = ptr; } while (0)

#define GETADDR_MODE_ZPG(addr, cpu, ptr, page_crossed) do { addr = (uint16_t)cpu_read8(cpu, ptr); } while (0)
#define GETADDR_MODE_ZPX(addr, cpu, ptr, page_crossed) do { addr = (uint16_t)((cpu_read8(cpu, ptr) + cpu->x) & 0xFF); } while (0)
#define GETADDR_MODE_ZPY(addr, cpu, ptr, page_crossed) do { addr = (uint16_t)((cpu_read8(cpu, ptr) + cpu->y) & 0xFF); } while (0)

#define GETADDR_MODE_ABS(addr, cpu, ptr, page_crossed) do { addr = cpu_read16(cpu, ptr); } while (0)
#define GETADDR_MODE_ABX(addr, cpu, ptr, page_crossed) do {\
    uint16_t base = cpu_read16(cpu, ptr);\
    addr = base + (uint16_t)cpu->x; \
    page_crossed = check_page_crossed(base, addr); } while (0)
#define GETADDR_MODE_ABY(addr, cpu, ptr, page_crossed) do {\
    uint16_t base = cpu_read16(cpu, ptr);\
    addr = base + (uint16_t)cpu->y; \
    page_crossed = check_page_crossed(base, addr); } while (0)

#define GETADDR_MODE_REL(addr, cpu, ptr, page_crossed) do {\
    int8_t offset = cpu_read8(cpu, ptr); \
    addr = cpu->pc + 1 + (uint16_t)((int16_t)offset); \
    page_crossed = check_page_crossed(cpu->pc + 1, addr);} while (0)

// 6502 bug mode with with page boundary:
// if address $3000 contains $40, $30FF contains $80, and $3100 contains $50,
// the result of JMP ($30FF) will be a transfer of control to $4080 rather than $5080 as you intended
// i.e. the 6502 took the low byte of the address from $30FF and the high byte from $3000
#define GETADDR_MODE_IND(addr, cpu, ptr, page_crossed) do { \
    uint16_t mem_address = cpu_read16(cpu, ptr); \
    uint16_t indirect_ref; \
    if ((mem_address & 0x00FF) == 0x00FF) {\
        uint16_t lo = (uint16_t)cpu_read8(cpu, mem_address); \
        uint16_t hi = (uint16_t)cpu_read8(cpu, mem_address & 0xFF00); \
        indirect_ref = ((hi << 8) | lo); \
    } else {\
        indirect_ref = cpu_read16(cpu, mem_address); \
    }\
    addr = indirect_ref; } while (0)
#define GETADDR_MODE_INX(addr, cpu, ptr, page_crossed) do {\
    uint8_t base = cpu_read8(cpu, ptr); \
    base += cpu->x;\
    addr = cpu_read16(cpu, (uint16_t)base); } while (0)
#define GETADDR_MODE_INY(addr, cpu, ptr, page_crossed) do {\
    uint16_t base = cpu_read16(cpu, (uint16_t)cpu_read8(cpu, ptr)); \
    addr = base + cpu->y; \
    page_crossed = check_page_crossed(base, addr); } while (0)




#define GETLENGTH_MODE_IMP 1
#define GETLENGTH_MODE_ACC 1
#define GETLENGTH_MODE_IMM 2
#define GETLENGTH_MODE_ZPG 2
#define GETLENGTH_MODE_ZPX 2
#define GETLENGTH_MODE_ZPY 2
#define GETLENGTH_MODE_ABS 3
#define GETLENGTH_MODE_ABX 3
#define GETLENGTH_MODE_ABY 3
#define GETLENGTH_MODE_REL 2
#define GETLENGTH_MODE_IND 3
#define GETLENGTH_MODE_INX 2
#define GETLENGTH_MODE_INY 2


// #define OP_GETMODE(...) OP_GETMODE_HEX(__VA_ARGS__)
// #define OP_GETMODE_HEX(OPCODE_HEX) OP_MODE_##OPCODE_HEX
#define OP_GETMODE(OPCODE_HEX) MACRO_CONCAT(OP_MODE_, OPCODE_HEX)

// #define OP_GETADDR_S(ADDRMODE) GETADDR_##ADDRMODE
// #define OP_GETADDR(...) OP_GETADDR_S(__VA_ARGS__)
// #define ADDRMODE_GETADDR_S(ADDRMODE) GETADDR_##ADDRMODE
// #define ADDRMODE_GETADDR(...) ADDRMODE_GETADDR_S(__VA_ARGS__)
#define ADDRMODE_GETADDR(MODE) MACRO_CONCAT(GETADDR_, MODE)
#define OP_GETADDR(OPCODE_HEX) MACRO_CONCAT(GETADDR_, OP_GETMODE(OPCODE_HEX))

// #define OP_GETLENGTH_S(RES) GETLENGTH_##RES
// #define OP_GETLENGTH(...) OP_GETLENGTH_S(__VA_ARGS__)
#define OP_GETLENGTH(OPCODE_HEX) MACRO_CONCAT(GETLENGTH_, OP_GETMODE(OPCODE_HEX))

// DO NOT CALL, no external linkage is generated 
inline void cpu_opcode_addrmode_test(void) {
    // _Static_assert(OP_GETLENGTH(OP_GETMODE_HEX(0x09)) == 2, "");
    // OP_GETADDR(OP_GETMODE_HEX(0x6A))(addr, cpu, ptr, page_crossed);
    _Static_assert(OP_GETLENGTH(0x09) == 2, "");
    OP_GETADDR(0x6A)(addr, cpu, ptr, page_crossed);
}
