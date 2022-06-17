#pragma once 
#include "sdk.h"
#include "cpu_addressmode.h"
#include "cpu_instruction.h"



/// the instruction information for an opcode 
/// since this will be heavily referenced, 
/// we want to make it as short as possible. 
struct cpu_opcode {
    uint8_t instruction; // enum cpu_instruction
    uint8_t addrmode;    // enum addressmode
    uint8_t length; 
    uint8_t base_cycles; 
};


extern const struct cpu_opcode opcode_table[256]; 
extern const char opcode_nametable[256][4]; 


void print_cpu_opcode_nametable(void); 


