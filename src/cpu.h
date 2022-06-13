#pragma once 

#include "sdk.h"

struct nescpu {
    uint8_t a; 
    uint8_t x; 
    uint8_t y; 
    uint8_t sp; 
    uint8_t p; 
    uint16_t pc; 
};

enum cpu_flags {
    cpu_flag_carry             = (0b00000001),
    cpu_flag_zero              = (0b00000010),
    cpu_flag_interrupt_disable = (0b00000100),
    cpu_flag_decimal_mode      = (0b00001000), // not supported 
    cpu_flag_break             = (0b00010000),
    cpu_flag_break2            = (0b00100000),
    cpu_flag_overflow          = (0b01000000), 
    cpu_flag_negative          = (0b10000000)
};
#define CPU_SP_RESET UINT8_C(0xFD)

void cpu_init(struct nescpu *cpu); 
void cpu_copy(struct nescpu *to_cpu, const struct nescpu *cpu); 
void cpu_reset(struct nescpu *cpu); 
void cpu_run(struct nescpu *cpu, void(*callback)(struct nescpu *c)); 

uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr); 
void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value); 

uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr); 
void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value); 




