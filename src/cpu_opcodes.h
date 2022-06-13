#pragma once 

enum addressmode {
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
    addressmode_accumulator, 
};


enum cpu_instruction {
    INVALID, 
    ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC, 
    CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP, 
    JSR, LDA, LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI, 
    RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA, 

    ////// unofficial ////// 
    /// combined operations 
    _ALR, _ANC, _ARR, _AXS, _LAX, _SAX, 
    /// RMW instructions 
    _DCP, _ISC, _RLA, _RRA, _SLO, _SRE, 
    /// Duplicated instructions
    _ADC, _SBC, 
    /// NOPs 
    _NOP, _SKB, _IGN, _CLD, _CLV, _SED, 
};

struct cpu_opcode {
    enum cpu_instruction instruction; 
    enum addressmode addrmode; 
    unsigned length; 
    unsigned base_cycles; 
    char name[5]; 
};


extern const struct cpu_opcode opcode_table[256]; 


