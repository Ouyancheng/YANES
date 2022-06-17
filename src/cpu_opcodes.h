#pragma once 
#include "sdk.h"
// #ifdef __GNUC__
// #define PACKED __attribute__((__packed__))
// #else 
// #define PACKED 
// #endif 
enum /*PACKED*/ addressmode {
    addressmode_absolute, 
    addressmode_zeropage, 
    addressmode_zeropage_x, 
    addressmode_zeropage_y, 
    addressmode_absolute_x, 
    addressmode_absolute_y, 
    addressmode_immediate, 
    addressmode_relative, 
    addressmode_implicit, 
    addressmode_indirect, 
    addressmode_indirect_x, 
    addressmode_indirect_y, 
    addressmode_accumulator
};


enum /*PACKED*/ cpu_instruction {
    INVALID, 
    ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC, 
    CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP, 
    JSR, LDA, LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI, 
    RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA, 

    _UNOFFICIAL_INSTRUCTION_START = 63, 
    // in this case you can detect if an instruction is unofficial by merely comparing its instruction value with _UNOFFICIAL_INSTRUCTION_START.  
    ////// unofficial ////// 
    /// combined operations 
    _ALR, _ANC, _ARR, _AXS, _LAX, _SAX, 
    /// RMW instructions 
    _DCP, _ISC, _RLA, _RRA, _SLO, _SRE, 
    /// Duplicated instructions
    _ADC, _SBC, 
    /// NOPs 
    _NOP, _SKB, _IGN, _CLD, _CLV, _SED
};

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


