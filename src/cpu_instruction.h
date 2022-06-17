#pragma once
#include "macros.h"
enum cpu_instruction {
    ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC, 
    CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP, 
    JSR, LDA, LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI, 
    RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA, 
    /* shift instructions with accumulator as argument */
    ASL_A, LSR_A, ROL_A, ROR_A, 
    _UNOFFICIAL_INSTRUCTION_START = 63, 
    /*** unofficial ***/
    /* combined operations */
    ALR_, ANC_, ARR_, AXS_, LAX_, SAX_, XAA_, AHX_, TAS_, LAS_,
    /* RMW instructions */
    DCP_, ISC_, RLA_, RRA_, SLO_, SRE_, SHY_, SHX_,
    /* Duplicated instructions */
    SBC_, 
    /* NOPs */
    NOP_, STP_
};
/*
You can detect if an instruction is unofficial 
by merely comparing its instruction value with _UNOFFICIAL_INSTRUCTION_START 
*/ 

#define OP_INSTR_0x00 (BRK, 7)
#define OP_INSTR_0x20 (JSR, 6)
#define OP_INSTR_0x40 (RTI, 6)
#define OP_INSTR_0x60 (RTS, 6)
#define OP_INSTR_0x80 (NOP_, 2)
#define OP_INSTR_0xA0 (LDY, 2)
#define OP_INSTR_0xC0 (CPY, 2)
#define OP_INSTR_0xE0 (CPX, 2)

#define OP_INSTR_0x04 (NOP_, 3)
#define OP_INSTR_0x24 (BIT, 3)
#define OP_INSTR_0x44 (NOP_, 3)
#define OP_INSTR_0x64 (NOP_, 3)
#define OP_INSTR_0x84 (STY, 3)
#define OP_INSTR_0xA4 (LDY, 3)
#define OP_INSTR_0xC4 (CPY, 3)
#define OP_INSTR_0xE4 (CPX, 3)

#define OP_INSTR_0x08 (PHP, 3)
#define OP_INSTR_0x28 (PLP, 4)
#define OP_INSTR_0x48 (PHA, 3)
#define OP_INSTR_0x68 (PLA, 4)
#define OP_INSTR_0x88 (DEY, 2)
#define OP_INSTR_0xA8 (TAY, 2)
#define OP_INSTR_0xC8 (INY, 2)
#define OP_INSTR_0xE8 (INX, 2)

#define OP_INSTR_0x0C (NOP_, 4)
#define OP_INSTR_0x2C (BIT, 4)
#define OP_INSTR_0x4C (JMP, 3)
#define OP_INSTR_0x6C (JMP, 5)
#define OP_INSTR_0x8C (STY, 4)
#define OP_INSTR_0xAC (LDY, 4)
#define OP_INSTR_0xCC (CPY, 4)
#define OP_INSTR_0xEC (CPX, 4)

#define OP_INSTR_0x10 (BPL, 2)
#define OP_INSTR_0x30 (BMI, 2)
#define OP_INSTR_0x50 (BVC, 2)
#define OP_INSTR_0x70 (BVS, 2)
#define OP_INSTR_0x90 (BCC, 2)
#define OP_INSTR_0xB0 (BCS, 2)
#define OP_INSTR_0xD0 (BNE, 2)
#define OP_INSTR_0xF0 (BEQ, 2)

#define OP_INSTR_0x14 (NOP_, 4)
#define OP_INSTR_0x34 (NOP_, 4)
#define OP_INSTR_0x54 (NOP_, 4)
#define OP_INSTR_0x74 (NOP_, 4)
#define OP_INSTR_0x94 (STY, 4)
#define OP_INSTR_0xB4 (LDY, 4)
#define OP_INSTR_0xD4 (NOP_, 4)
#define OP_INSTR_0xF4 (NOP_, 4)

#define OP_INSTR_0x18 (CLC, 2)
#define OP_INSTR_0x38 (SEC, 2)
#define OP_INSTR_0x58 (CLI, 2)
#define OP_INSTR_0x78 (SEI, 2)
#define OP_INSTR_0x98 (TYA, 2)
#define OP_INSTR_0xB8 (CLV, 2)
#define OP_INSTR_0xD8 (CLD, 2)
#define OP_INSTR_0xF8 (SED, 2)

#define OP_INSTR_0x1C (NOP_, 4) /*5?*/
#define OP_INSTR_0x3C (NOP_, 4) /*5?*/
#define OP_INSTR_0x5C (NOP_, 4) /*5?*/
#define OP_INSTR_0x7C (NOP_, 4) /*5?*/
#define OP_INSTR_0x9C (SHY_, 4) /*5?*/
#define OP_INSTR_0xBC (LDY, 4)  /*+1 if page crossed*/
#define OP_INSTR_0xDC (NOP_, 4) /*5?*/
#define OP_INSTR_0xFC (NOP_, 4) /*5?*/

///////////////////////////////////////////////////////////
#define OP_INSTR_0x01 (ORA, 6)
#define OP_INSTR_0x21 (AND, 6)
#define OP_INSTR_0x41 (EOR, 6)
#define OP_INSTR_0x61 (ADC, 6)
#define OP_INSTR_0x81 (STA, 6)
#define OP_INSTR_0xA1 (LDA, 6)
#define OP_INSTR_0xC1 (CMP, 6)
#define OP_INSTR_0xE1 (SBC, 6)

#define OP_INSTR_0x05 (ORA, 3)
#define OP_INSTR_0x25 (AND, 3)
#define OP_INSTR_0x45 (EOR, 3)
#define OP_INSTR_0x65 (ADC, 3)
#define OP_INSTR_0x85 (STA, 3)
#define OP_INSTR_0xA5 (LDA, 3)
#define OP_INSTR_0xC5 (CMP, 3)
#define OP_INSTR_0xE5 (SBC, 3)

#define OP_INSTR_0x09 (ORA, 2)
#define OP_INSTR_0x29 (AND, 2)
#define OP_INSTR_0x49 (EOR, 2)
#define OP_INSTR_0x69 (ADC, 2)
#define OP_INSTR_0x89 (NOP_, 2)
#define OP_INSTR_0xA9 (LDA, 2)
#define OP_INSTR_0xC9 (CMP, 2)
#define OP_INSTR_0xE9 (SBC, 2)

#define OP_INSTR_0x0D (ORA, 4)
#define OP_INSTR_0x2D (AND, 4)
#define OP_INSTR_0x4D (EOR, 4)
#define OP_INSTR_0x6D (ADC, 4)
#define OP_INSTR_0x8D (STA, 4)
#define OP_INSTR_0xAD (LDA, 4)
#define OP_INSTR_0xCD (CMP, 4)
#define OP_INSTR_0xED (SBC, 4)

#define OP_INSTR_0x11 (ORA, 5) /*+1 if page crossed*/
#define OP_INSTR_0x31 (AND, 5) /*+1 if page crossed*/
#define OP_INSTR_0x51 (EOR, 5) /*+1 if page crossed*/
#define OP_INSTR_0x71 (ADC, 5) /*+1 if page crossed*/
#define OP_INSTR_0x91 (STA, 6) 
#define OP_INSTR_0xB1 (LDA, 5) /*+1 if page crossed*/
#define OP_INSTR_0xD1 (CMP, 5) /*+1 if page crossed*/
#define OP_INSTR_0xF1 (SBC, 5) /*+1 if page crossed*/

#define OP_INSTR_0x15 (ORA, 4)
#define OP_INSTR_0x35 (AND, 4)
#define OP_INSTR_0x55 (EOR, 4)
#define OP_INSTR_0x75 (ADC, 4)
#define OP_INSTR_0x95 (STA, 4)
#define OP_INSTR_0xB5 (LDA, 4)
#define OP_INSTR_0xD5 (CMP, 4)
#define OP_INSTR_0xF5 (SBC, 4)

#define OP_INSTR_0x19 (ORA, 4) /*+1 if page crossed*/
#define OP_INSTR_0x39 (AND, 4) /*+1 if page crossed*/
#define OP_INSTR_0x59 (EOR, 4) /*+1 if page crossed*/
#define OP_INSTR_0x79 (ADC, 4) /*+1 if page crossed*/
#define OP_INSTR_0x99 (STA, 5)
#define OP_INSTR_0xB9 (LDA, 4) /*+1 if page crossed*/
#define OP_INSTR_0xD9 (CMP, 4) /*+1 if page crossed*/
#define OP_INSTR_0xF9 (SBC, 4) /*+1 if page crossed*/

#define OP_INSTR_0x1D (ORA, 4) /*+1 if page crossed*/
#define OP_INSTR_0x3D (AND, 4) /*+1 if page crossed*/
#define OP_INSTR_0x5D (EOR, 4) /*+1 if page crossed*/
#define OP_INSTR_0x7D (ADC, 4) /*+1 if page crossed*/
#define OP_INSTR_0x9D (STA, 5)
#define OP_INSTR_0xBD (LDA, 4) /*+1 if page crossed*/
#define OP_INSTR_0xDD (CMP, 4) /*+1 if page crossed*/
#define OP_INSTR_0xFD (SBC, 4) /*+1 if page crossed*/

///////////////////////////////////////////////////////////
#define OP_INSTR_0x02 (STP_, 2)
#define OP_INSTR_0x22 (STP_, 2)
#define OP_INSTR_0x42 (STP_, 2)
#define OP_INSTR_0x62 (STP_, 2)
#define OP_INSTR_0x82 (NOP_, 2)
#define OP_INSTR_0xA2 (LDX, 2)
#define OP_INSTR_0xC2 (NOP_, 2)
#define OP_INSTR_0xE2 (NOP_, 2)

#define OP_INSTR_0x06 (ASL, 5)
#define OP_INSTR_0x26 (ROL, 5)
#define OP_INSTR_0x46 (LSR, 5)
#define OP_INSTR_0x66 (ROR, 5)
#define OP_INSTR_0x86 (STX, 3)
#define OP_INSTR_0xA6 (LDX, 3)
#define OP_INSTR_0xC6 (DEC, 5)
#define OP_INSTR_0xE6 (INC, 5)

#define OP_INSTR_0x0A (ASL_A, 2)
#define OP_INSTR_0x2A (ROL_A, 2)
#define OP_INSTR_0x4A (LSR_A, 2)
#define OP_INSTR_0x6A (ROR_A, 2)
#define OP_INSTR_0x8A (TXA, 2)
#define OP_INSTR_0xAA (TAX, 2)
#define OP_INSTR_0xCA (DEX, 2)
#define OP_INSTR_0xEA (NOP, 2)

#define OP_INSTR_0x0E (ASL, 6)
#define OP_INSTR_0x2E (ROL, 6)
#define OP_INSTR_0x4E (LSR, 6)
#define OP_INSTR_0x6E (ROR, 6)
#define OP_INSTR_0x8E (STX, 4)
#define OP_INSTR_0xAE (LDX, 4)
#define OP_INSTR_0xCE (DEC, 6)
#define OP_INSTR_0xEE (INC, 6)

#define OP_INSTR_0x12 (STP_, 2)
#define OP_INSTR_0x32 (STP_, 2)
#define OP_INSTR_0x52 (STP_, 2)
#define OP_INSTR_0x72 (STP_, 2)
#define OP_INSTR_0x92 (STP_, 2)
#define OP_INSTR_0xB2 (STP_, 2)
#define OP_INSTR_0xD2 (STP_, 2)
#define OP_INSTR_0xF2 (STP_, 2)

#define OP_INSTR_0x16 (ASL, 6)
#define OP_INSTR_0x36 (ROL, 6)
#define OP_INSTR_0x56 (LSR, 6)
#define OP_INSTR_0x76 (ROR, 6)
#define OP_INSTR_0x96 (STX, 4)
#define OP_INSTR_0xB6 (LDX, 4)
#define OP_INSTR_0xD6 (DEC, 6)
#define OP_INSTR_0xF6 (INC, 6)

#define OP_INSTR_0x1A (NOP_, 2)
#define OP_INSTR_0x3A (NOP_, 2)
#define OP_INSTR_0x5A (NOP_, 2)
#define OP_INSTR_0x7A (NOP_, 2)
#define OP_INSTR_0x9A (TXS, 2)
#define OP_INSTR_0xBA (TSX, 2)
#define OP_INSTR_0xDA (NOP_, 2)
#define OP_INSTR_0xFA (NOP_, 2)

#define OP_INSTR_0x1E (ASL, 7)
#define OP_INSTR_0x3E (ROL, 7)
#define OP_INSTR_0x5E (LSR, 7)
#define OP_INSTR_0x7E (ROR, 7)
#define OP_INSTR_0x9E (SHX_, 4) /*5?*/
#define OP_INSTR_0xBE (LDX, 4) /*+1 if page crossed*/
#define OP_INSTR_0xDE (DEC, 7)
#define OP_INSTR_0xFE (INC, 7)

///////////////////////////////////////////////////////////
#define OP_INSTR_0x03 (SLO_, 8)
#define OP_INSTR_0x23 (RLA_, 8)
#define OP_INSTR_0x43 (SRE_, 8)
#define OP_INSTR_0x63 (RRA_, 8)
#define OP_INSTR_0x83 (SAX_, 6)
#define OP_INSTR_0xA3 (LAX_, 6)
#define OP_INSTR_0xC3 (DCP_, 8)
#define OP_INSTR_0xE3 (ISC_, 8)

#define OP_INSTR_0x07 (SLO_, 5)
#define OP_INSTR_0x27 (RLA_, 5)
#define OP_INSTR_0x47 (SRE_, 5)
#define OP_INSTR_0x67 (RRA_, 5)
#define OP_INSTR_0x87 (SAX_, 3)
#define OP_INSTR_0xA7 (LAX_, 3)
#define OP_INSTR_0xC7 (DCP_, 5)
#define OP_INSTR_0xE7 (ISC_, 5)

#define OP_INSTR_0x0B (ANC_, 2)
#define OP_INSTR_0x2B (ANC_, 2)
#define OP_INSTR_0x4B (ALR_, 2)
#define OP_INSTR_0x6B (ARR_, 2)
#define OP_INSTR_0x8B (XAA_, 2)
#define OP_INSTR_0xAB (LAX_, 2)
#define OP_INSTR_0xCB (AXS_, 2)
#define OP_INSTR_0xEB (SBC_, 2)

#define OP_INSTR_0x0F (SLO_, 6)
#define OP_INSTR_0x2F (RLA_, 6)
#define OP_INSTR_0x4F (SRE_, 6)
#define OP_INSTR_0x6F (RRA_, 6)
#define OP_INSTR_0x8F (SAX_, 4)
#define OP_INSTR_0xAF (LAX_, 4)
#define OP_INSTR_0xCF (DCP_, 6)
#define OP_INSTR_0xEF (ISC_, 6)

#define OP_INSTR_0x13 (SLO_, 8)
#define OP_INSTR_0x33 (RLA_, 8)
#define OP_INSTR_0x53 (SRE_, 8)
#define OP_INSTR_0x73 (RRA_, 8)
#define OP_INSTR_0x93 (AHX_, 6)
#define OP_INSTR_0xB3 (LAX_, 5) /*+1 if page crossed?*/
#define OP_INSTR_0xD3 (DCP_, 8)
#define OP_INSTR_0xF3 (ISC_, 8)

#define OP_INSTR_0x17 (SLO_, 6)
#define OP_INSTR_0x37 (RLA_, 6)
#define OP_INSTR_0x57 (SRE_, 6)
#define OP_INSTR_0x77 (RRA_, 6)
#define OP_INSTR_0x97 (SAX_, 4)
#define OP_INSTR_0xB7 (LAX_, 4)
#define OP_INSTR_0xD7 (DCP_, 6)
#define OP_INSTR_0xF7 (ISC_, 6)

#define OP_INSTR_0x1B (SLO_, 7)
#define OP_INSTR_0x3B (RLA_, 7)
#define OP_INSTR_0x5B (SRE_, 7)
#define OP_INSTR_0x7B (RRA_, 7)
#define OP_INSTR_0x9B (TAS_, 5)
#define OP_INSTR_0xBB (LAS_, 4) /*+1 if page crossed?*/
#define OP_INSTR_0xDB (DCP_, 7)
#define OP_INSTR_0xFB (ISC_, 7)

#define OP_INSTR_0x1F (SLO_, 7)
#define OP_INSTR_0x3F (RLA_, 7)
#define OP_INSTR_0x5F (SRE_, 7)
#define OP_INSTR_0x7F (RRA_, 7)
#define OP_INSTR_0x9F (AHX_, 5)
#define OP_INSTR_0xBF (LAX_, 4) /*+1 if page crossed?*/
#define OP_INSTR_0xDF (DCP_, 7)
#define OP_INSTR_0xFF (ISC_, 7)

///////////////////////////////////////////////////////////

// #define OP_GETINSTR_TUPLE(...) OP_GETINSTR_TUPLE_S(__VA_ARGS__)
// #define OP_GETINSTR_TUPLE_S(OPCODE_HEX) OP_INSTR_##OPCODE_HEX
#define OP_GETINSTR_TUPLE(OPCODE_HEX) MACRO_CONCAT(OP_INSTR_, OPCODE_HEX)

// #define OP_GETINSTR(...) OP_GETINSTR_S(__VA_ARGS__)
// #define OP_GETINSTR_S(OPCODE_HEX) TUPLE_APPLY(TUPLE_FST, OP_GETINSTR_TUPLE(OPCODE_HEX))
#define OP_GETINSTR(OPCODE_HEX) TUPLE_APPLY(TUPLE_FST, OP_GETINSTR_TUPLE(OPCODE_HEX))

// #define OP_GETCYC(...) OP_GETCYC_S(__VA_ARGS__)
// #define OP_GETCYC_S(OPCODE_HEX) TUPLE_APPLY(TUPLE_SND, OP_GETINSTR_TUPLE(OPCODE_HEX))
#define OP_GETCYC(OPCODE_HEX) TUPLE_APPLY(TUPLE_SND, OP_GETINSTR_TUPLE(OPCODE_HEX))

// #define OP_EXECINSTR(...) OP_EXECINSTR_S(__VA_ARGS__)
// #define OP_EXECINSTR_S(INSTR) exec_##INSTR
#define EXECINSTR(INSTR) MACRO_CONCAT(exec_, INSTR)

// #define INSTR_TO_STR(...) INSTR_TO_STR_S(__VA_ARGS__)
// #define INSTR_TO_STR_S(INSTR) #INSTR
#define INSTR_TO_STR(INSTR) MACRO_EXPAND_TO_STR(INSTR)

inline void __test_instruction(void) {
    _Static_assert(OP_GETINSTR(0x00) == BRK, "0x00 is not brk");
    _Static_assert(OP_GETCYC(0x00) == 7, "BRK is not cycle 7"); 
}
