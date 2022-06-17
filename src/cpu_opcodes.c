#include "cpu_opcodes.h"

#define OP(INSTR, MODE, LEN, CYC) {.instruction=INSTR, .addrmode=MODE, .length=LEN, .base_cycles=CYC}
/**
 * This table describes the instructions, the address mode, the instruction length and the base cycles the instruction takes 
 * Reference: http://www.6502.org/tutorials/6502opcodes.html 
 */
const struct cpu_opcode opcode_table[256] = {
    // ADC (ADd with Carry) Affects Flags: N V Z C
    [0x69] = OP(ADC, MODE_IMM, 2, 2), 
    [0x65] = OP(ADC, MODE_ZPG, 2, 3),  
    [0x75] = OP(ADC, MODE_ZPX, 2, 4),  
    [0x6D] = OP(ADC, MODE_ABS, 3, 4), 
    [0x7D] = OP(ADC, MODE_ABX, 3, 4),  // cyc=4+if page boundary crossed
    [0x79] = OP(ADC, MODE_ABY, 3, 4),  // cyc=4+
    [0x61] = OP(ADC, MODE_INX, 2, 6),  
    [0x71] = OP(ADC, MODE_INY, 2, 5),  // cyc=5+
    // AND (bitwise AND with accumulator) Affects Flags: N Z
    [0x29] = OP(AND, MODE_IMM, 2, 2),  
    [0x25] = OP(AND, MODE_ZPG, 2, 3),  
    [0x35] = OP(AND, MODE_ZPX, 2, 4),  
    [0x2D] = OP(AND, MODE_ABS, 3, 4),  
    [0x3D] = OP(AND, MODE_ABX, 3, 4),  // cyc=4+if page boundary crossed 
    [0x39] = OP(AND, MODE_ABY, 3, 4),  // cyc=4+
    [0x21] = OP(AND, MODE_INX, 2, 6),  
    [0x31] = OP(AND, MODE_INY, 2, 5),  // cyc=5+
    // ASL (Arithmetic Shift Left) Affects Flags: N Z C
    // ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    [0x0A] = OP(ASL, MODE_ACC, 1, 2),  // ASL A with Accumulator as argument 
    [0x06] = OP(ASL, MODE_ZPG, 2, 5),  
    [0x16] = OP(ASL, MODE_ZPX, 2, 6),  
    [0x0E] = OP(ASL, MODE_ABS, 3, 6),  
    [0x1E] = OP(ASL, MODE_ABX, 3, 7),  
    // BIT (test BITs) Affects Flags: N V Z 
    // BIT sets the Z flag as though the value in the address tested were ANDed with the accumulator. 
    // The N and V flags are set to match bits 7 and 6 respectively in the value stored at the tested address.
    [0x24] = OP(BIT, MODE_ZPG, 2, 3),  
    [0x2C] = OP(BIT, MODE_ABS, 3, 4),  
    // Branch Instructions
    // A branch not taken requires two machine cycles. Add one if the branch is taken and add one more if the branch crosses a page boundary.
    [0x10] = OP(BPL, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0x30] = OP(BMI, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0x50] = OP(BVC, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0x70] = OP(BVS, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0x90] = OP(BCC, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0xB0] = OP(BCS, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0xD0] = OP(BNE, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    [0xF0] = OP(BEQ, MODE_REL, 2, 2),  // cyc+1 if taken, +1 if cross page 
    // BRK (BReaK)
    // Affects Flags: B
    // BRK causes a non-maskable interrupt and increments the program counter by one. 
    // Therefore an RTI will go to the address of the BRK +2 
    // so that BRK may be used to replace a two-byte instruction for debugging and the subsequent RTI will be correct.
    [0x00] = OP(BRK, MODE_IMP, 1, 7),  
    // CMP (CoMPare accumulator)
    // Affects Flags: N Z C
    // Compare sets flags as if a subtraction had been carried out. 
    // If the value in the accumulator is equal or greater than the compared value, the Carry will be set. 
    // The equal (Z) and negative (N) flags will be set based on equality or lack thereof and the sign (i.e. A>=$80) of the accumulator.
    [0xC9] = OP(CMP, MODE_IMM, 2, 2),  
    [0xC5] = OP(CMP, MODE_ZPG, 2, 3),  
    [0xD5] = OP(CMP, MODE_ZPX, 2, 4),  
    [0xCD] = OP(CMP, MODE_ABS, 3, 4),  
    [0xDD] = OP(CMP, MODE_ABX, 3, 4),  // cyc+1 if page crossed 
    [0xD9] = OP(CMP, MODE_ABY, 3, 4),  // cyc+1 if page crossed 
    [0xC1] = OP(CMP, MODE_INX, 2, 6),  
    [0xD1] = OP(CMP, MODE_INY, 2, 5),  // cyc+1 if page crossed 
    // CPX (ComPare X register) 
    // Affects Flags: N Z C
    [0xE0] = OP(CPX, MODE_IMM, 2, 2),  
    [0xE4] = OP(CPX, MODE_ZPG, 2, 3),  
    [0xEC] = OP(CPX, MODE_ABS, 3, 4),  
    // CPY (ComPare Y register)
    // Affects Flags: N Z C
    [0xC0] = OP(CPY, MODE_IMM, 2, 2),  
    [0xC4] = OP(CPY, MODE_ZPG, 2, 3),  
    [0xCC] = OP(CPY, MODE_ABS, 3, 4),  
    // DEC (DECrement memory)
    // Affects Flags: N Z
    [0xC6] = OP(DEC, MODE_ZPG, 2, 5),  
    [0xD6] = OP(DEC, MODE_ZPX, 2, 6),  
    [0xCE] = OP(DEC, MODE_ABS, 3, 6),  
    [0xDE] = OP(DEC, MODE_ABX, 3, 7),  
    // EOR (bitwise Exclusive OR)
    // Affects Flags: N Z
    [0x49] = OP(EOR, MODE_IMM, 2, 2),  
    [0x45] = OP(EOR, MODE_ZPG, 2, 3),  
    [0x55] = OP(EOR, MODE_ZPX, 2, 4),  
    [0x4D] = OP(EOR, MODE_ABS, 3, 4),  
    [0x5D] = OP(EOR, MODE_ABX, 3, 4),  // cyc+1 if page crossed
    [0x59] = OP(EOR, MODE_ABY, 3, 4),  // cyc+1 if page crossed
    [0x41] = OP(EOR, MODE_INX, 2, 6),  
    [0x51] = OP(EOR, MODE_INY, 2, 5),  // cyc+1 if page crossed 
    // Flag (Processor Status) Instructions
    [0x18] = OP(CLC, MODE_IMP, 1, 2),  
    [0x38] = OP(SEC, MODE_IMP, 1, 2),  
    [0x58] = OP(CLI, MODE_IMP, 1, 2),  
    [0x78] = OP(SEI, MODE_IMP, 1, 2),  
    [0xB8] = OP(CLV, MODE_IMP, 1, 2),  
    [0xD8] = OP(CLD, MODE_IMP, 1, 2),  
    [0xF8] = OP(SED, MODE_IMP, 1, 2),  
    // INC (INCrement memory)
    // Affects Flags: N Z
    [0xE6] = OP(INC, MODE_ZPG, 2, 5),  
    [0xF6] = OP(INC, MODE_ZPX, 2, 6),  
    [0xEE] = OP(INC, MODE_ABS, 3, 6),  
    [0xFE] = OP(INC, MODE_ABX, 3, 7),  
    // JMP (JuMP)
    // Affects Flags: none 
    [0x4C] = OP(JMP, MODE_ABS, 3, 3),  
    [0x6C] = OP(JMP, MODE_IND, 3, 5),  
    // AN INDIRECT JUMP MUST NEVER USE A
    // VECTOR BEGINNING ON THE LAST BYTE
    // OF A PAGE
    // For example 
    // if address $3000 contains $40, 
    // $30FF contains $80, 
    // and $3100 contains $50, 
    // the result of JMP ($30FF) will be a transfer of control to $4080 rather than $5080 as you intended 
    // i.e. the 6502 took the low byte of the address from $30FF and the high byte from $3000.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // JSR (Jump to SubRoutine)
    // Affects Flags: none
    [0x20] = OP(JSR, MODE_ABS, 3, 6),  
    // LDA (LoaD Accumulator)
    // Affects Flags: N Z
    [0xA9] = OP(LDA, MODE_IMM, 2, 2),  
    [0xA5] = OP(LDA, MODE_ZPG, 2, 3),  
    [0xB5] = OP(LDA, MODE_ZPX, 2, 4),  
    [0xAD] = OP(LDA, MODE_ABS, 3, 4),  
    [0xBD] = OP(LDA, MODE_ABX, 3, 4),  // cyc+1 if page crossed 
    [0xB9] = OP(LDA, MODE_ABY, 3, 4),  // cyc+1 if page crossed 
    [0xA1] = OP(LDA, MODE_INX, 2, 6),  
    [0xB1] = OP(LDA, MODE_INY, 2, 5),  // cyc+1 if page crossed 
    // LDX (LoaD X register)
    // Affects Flags: N Z
    [0xA2] = OP(LDX, MODE_IMM, 2, 2),  
    [0xA6] = OP(LDX, MODE_ZPG, 2, 3),  
    [0xB6] = OP(LDX, MODE_ZPY, 2, 4),  
    [0xAE] = OP(LDX, MODE_ABS, 3, 4),  
    [0xBE] = OP(LDX, MODE_ABY, 3, 4),  // cyc+1 if page crossed 
    // LDY (LoaD Y register)
    // Affects Flags: N Z
    [0xA0] = OP(LDY, MODE_IMM, 2, 2),  
    [0xA4] = OP(LDY, MODE_ZPG, 2, 3),  
    [0xB4] = OP(LDY, MODE_ZPX, 2, 4),  
    [0xAC] = OP(LDY, MODE_ABS, 3, 4),  
    [0xBC] = OP(LDY, MODE_ABX, 3, 4),  // cyc+1 if page crossed 
    // LSR (Logical Shift Right)
    // Affects Flags: N Z C
    [0x4A] = OP(LSR, MODE_ACC, 1, 2),  // LSR A 
    [0x46] = OP(LSR, MODE_ZPG, 2, 5),  
    [0x56] = OP(LSR, MODE_ZPX, 2, 6),  
    [0x4E] = OP(LSR, MODE_ABS, 3, 6),  
    [0x5E] = OP(LSR, MODE_ABX, 3, 7),  
    // NOP (No OPeration)
    [0xEA] = OP(NOP, MODE_IMP, 1, 2),  
    // ORA (bitwise OR with Accumulator)
    // Affects Flags: N Z
    [0x09] = OP(ORA, MODE_IMM, 2, 2),  
    [0x05] = OP(ORA, MODE_ZPG, 2, 3),  
    [0x15] = OP(ORA, MODE_ZPX, 2, 4),  
    [0x0D] = OP(ORA, MODE_ABS, 3, 4),  
    [0x1D] = OP(ORA, MODE_ABX, 3, 4),  // cyc+1 if page crossed
    [0x19] = OP(ORA, MODE_ABY, 3, 4),  // cyc+1 if page crossed 
    [0x01] = OP(ORA, MODE_INX, 2, 6),  
    [0x11] = OP(ORA, MODE_INY, 2, 5),  // cyc+1 if page crossed 
    // Register Instructions
    // Affect Flags: N Z
    [0xAA] = OP(TAX, MODE_IMP, 1, 2),  
    [0x8A] = OP(TXA, MODE_IMP, 1, 2),  
    [0xCA] = OP(DEX, MODE_IMP, 1, 2),  
    [0xE8] = OP(INX, MODE_IMP, 1, 2),  
    [0xA8] = OP(TAY, MODE_IMP, 1, 2),  
    [0x98] = OP(TYA, MODE_IMP, 1, 2),  
    [0x88] = OP(DEY, MODE_IMP, 1, 2),  
    [0xC8] = OP(INY, MODE_IMP, 1, 2),  
    // ROL (ROtate Left)
    // Affects Flags: N Z C
    [0x2A] = OP(ROL, MODE_ACC, 1, 2),  
    [0x26] = OP(ROL, MODE_ZPG, 2, 5),  
    [0x36] = OP(ROL, MODE_ZPX, 2, 6),  
    [0x2E] = OP(ROL, MODE_ABS, 3, 6),  
    [0x3E] = OP(ROL, MODE_ABX, 3, 7),  
    // ROR (ROtate Right)
    // Affects Flags: N Z C
    [0x6A] = OP(ROR, MODE_ACC, 1, 2),  
    [0x66] = OP(ROR, MODE_ZPG, 2, 5),  
    [0x76] = OP(ROR, MODE_ZPX, 2, 6),  
    [0x6E] = OP(ROR, MODE_ABS, 3, 6),  
    [0x7E] = OP(ROR, MODE_ABX, 3, 7),  
    // RTI (ReTurn from Interrupt)
    // Affects Flags: all
    [0x40] = OP(RTI, MODE_IMP, 1, 6),  
    // RTS (ReTurn from Subroutine)
    // Affects Flags: none
    [0x60] = OP(RTS, MODE_IMP, 1, 6),  
    // SBC (SuBtract with Carry)
    // Affects Flags: N V Z C
    [0xE9] = OP(SBC, MODE_IMM, 2, 2),  
    [0xE5] = OP(SBC, MODE_ZPG, 2, 3),  
    [0xF5] = OP(SBC, MODE_ZPX, 2, 4),  
    [0xED] = OP(SBC, MODE_ABS, 3, 4),  
    [0xFD] = OP(SBC, MODE_ABX, 3, 4),  // cyc+1 if page crossed
    [0xF9] = OP(SBC, MODE_ABY, 3, 4),  // cyc+1 if page crossed 
    [0xE1] = OP(SBC, MODE_INX, 2, 6),  
    [0xF1] = OP(SBC, MODE_INY, 2, 5),  // cyc+1 if page crossed 
    // STA (STore Accumulator)
    // Affects Flags: none
    [0x85] = OP(STA, MODE_ZPG, 2, 3),  
    [0x95] = OP(STA, MODE_ZPX, 2, 4),  
    [0x8D] = OP(STA, MODE_ABS, 3, 4),  
    [0x9D] = OP(STA, MODE_ABX, 3, 5),  
    [0x99] = OP(STA, MODE_ABY, 3, 5),  
    [0x81] = OP(STA, MODE_INX, 2, 6),  
    [0x91] = OP(STA, MODE_INY, 2, 6),  
    // Stack Instructions
    [0x9A] = OP(TXS, MODE_IMP, 1, 2),  
    [0xBA] = OP(TSX, MODE_IMP, 1, 2),  
    [0x48] = OP(PHA, MODE_IMP, 1, 3),  
    [0x68] = OP(PLA, MODE_IMP, 1, 4),  
    [0x08] = OP(PHP, MODE_IMP, 1, 3), 
    [0x28] = OP(PLP, MODE_IMP, 1, 4),   
    // STX (STore X register)
    // Affects Flags: none
    [0x86] = OP(STX, MODE_ZPG, 2, 3),  
    [0x96] = OP(STX, MODE_ZPY, 2, 4),  
    [0x8E] = OP(STX, MODE_ABS, 3, 4),  
    // STY (STore Y register)
    // Affects Flags: none
    [0x84] = OP(STY, MODE_ZPG, 2, 3),  
    [0x94] = OP(STY, MODE_ZPX, 2, 4),  
    [0x8C] = OP(STY, MODE_ABS, 3, 4),  
};
