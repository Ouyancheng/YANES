#include "cpu_opcodes.h"

/**
 * This table describes the instructions, the address mode, the instruction length and the base cycles the instruction takes 
 * Reference: http://www.6502.org/tutorials/6502opcodes.html 
 */
const struct cpu_opcode opcode_table[256] = {
    // ADC (ADd with Carry) Affects Flags: N V Z C
    [0x69] = {.instruction=ADC, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="ADC"},
    [0x65] = {.instruction=ADC, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="ADC"}, 
    [0x75] = {.instruction=ADC, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="ADC"}, 
    [0x6D] = {.instruction=ADC, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="ADC"},
    [0x7D] = {.instruction=ADC, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="ADC"}, // cyc=4+if page boundary crossed
    [0x79] = {.instruction=ADC, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="ADC"}, // cyc=4+
    [0x61] = {.instruction=ADC, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="ADC"}, 
    [0x71] = {.instruction=ADC, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="ADC"}, // cyc=5+
    // AND (bitwise AND with accumulator) Affects Flags: N Z
    [0x29] = {.instruction=AND, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="AND"}, 
    [0x25] = {.instruction=AND, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="AND"}, 
    [0x35] = {.instruction=AND, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="AND"}, 
    [0x2D] = {.instruction=AND, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="AND"}, 
    [0x3D] = {.instruction=AND, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="AND"}, // cyc=4+if page boundary crossed 
    [0x39] = {.instruction=AND, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="AND"}, // cyc=4+
    [0x21] = {.instruction=AND, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="AND"}, 
    [0x31] = {.instruction=AND, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="AND"}, // cyc=5+
    // ASL (Arithmetic Shift Left) Affects Flags: N Z C
    // ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    [0x0A] = {.instruction=ASL, .addrmode=addressmode_accumulator,.length=1, .base_cycles=2, .name="ASL"}, // ASL A with Accumulator as argument 
    [0x06] = {.instruction=ASL, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=5, .name="ASL"}, 
    [0x16] = {.instruction=ASL, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=6, .name="ASL"}, 
    [0x0E] = {.instruction=ASL, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="ASL"}, 
    [0x1E] = {.instruction=ASL, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=7, .name="ASL"}, 
    // BIT (test BITs) Affects Flags: N V Z 
    // BIT sets the Z flag as though the value in the address tested were ANDed with the accumulator. 
    // The N and V flags are set to match bits 7 and 6 respectively in the value stored at the tested address.
    [0x24] = {.instruction=BIT, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="BIT"}, 
    [0x2C] = {.instruction=BIT, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="BIT"}, 
    // Branch Instructions
    // A branch not taken requires two machine cycles. Add one if the branch is taken and add one more if the branch crosses a page boundary.
    [0x10] = {.instruction=BPL, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BPL"}, // cyc+1 if taken, +1 if cross page 
    [0x30] = {.instruction=BMI, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BMI"}, // cyc+1 if taken, +1 if cross page 
    [0x50] = {.instruction=BVC, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BVC"}, // cyc+1 if taken, +1 if cross page 
    [0x70] = {.instruction=BVS, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BVS"}, // cyc+1 if taken, +1 if cross page 
    [0x90] = {.instruction=BCC, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BCC"}, // cyc+1 if taken, +1 if cross page 
    [0xB0] = {.instruction=BCS, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BCS"}, // cyc+1 if taken, +1 if cross page 
    [0xD0] = {.instruction=BNE, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BNE"}, // cyc+1 if taken, +1 if cross page 
    [0xF0] = {.instruction=BEQ, .addrmode=addressmode_relative,   .length=2, .base_cycles=2, .name="BEQ"}, // cyc+1 if taken, +1 if cross page 
    // BRK (BReaK)
    // Affects Flags: B
    // BRK causes a non-maskable interrupt and increments the program counter by one. 
    // Therefore an RTI will go to the address of the BRK +2 
    // so that BRK may be used to replace a two-byte instruction for debugging and the subsequent RTI will be correct.
    [0x00] = {.instruction=BRK, .addrmode=addressmode_implicit,   .length=1, .base_cycles=7, .name="BRK"}, 
    // CMP (CoMPare accumulator)
    // Affects Flags: N Z C
    // Compare sets flags as if a subtraction had been carried out. 
    // If the value in the accumulator is equal or greater than the compared value, the Carry will be set. 
    // The equal (Z) and negative (N) flags will be set based on equality or lack thereof and the sign (i.e. A>=$80) of the accumulator.
    [0xC9] = {.instruction=CMP, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="CMP"}, 
    [0xC5] = {.instruction=CMP, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="CMP"}, 
    [0xD5] = {.instruction=CMP, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="CMP"}, 
    [0xCD] = {.instruction=CMP, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="CMP"}, 
    [0xDD] = {.instruction=CMP, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="CMP"}, // cyc+1 if page crossed 
    [0xD9] = {.instruction=CMP, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="CMP"}, // cyc+1 if page crossed 
    [0xC1] = {.instruction=CMP, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="CMP"}, 
    [0xD1] = {.instruction=CMP, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="CMP"}, // cyc+1 if page crossed 
    // CPX (ComPare X register) 
    // Affects Flags: N Z C
    [0xE0] = {.instruction=CPX, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="CPX"}, 
    [0xE4] = {.instruction=CPX, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="CPX"}, 
    [0xEC] = {.instruction=CPX, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="CPX"}, 
    // CPY (ComPare Y register)
    // Affects Flags: N Z C
    [0xC0] = {.instruction=CPY, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="CPY"}, 
    [0xC4] = {.instruction=CPY, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="CPY"}, 
    [0xCC] = {.instruction=CPY, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="CPY"}, 
    // DEC (DECrement memory)
    // Affects Flags: N Z
    [0xC6] = {.instruction=DEC, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=5, .name="DEC"}, 
    [0xD6] = {.instruction=DEC, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=6, .name="DEC"}, 
    [0xCE] = {.instruction=DEC, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="DEC"}, 
    [0xDE] = {.instruction=DEC, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=7, .name="DEC"}, 
    // EOR (bitwise Exclusive OR)
    // Affects Flags: N Z
    [0x49] = {.instruction=EOR, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="EOR"}, 
    [0x45] = {.instruction=EOR, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="EOR"}, 
    [0x55] = {.instruction=EOR, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="EOR"}, 
    [0x4D] = {.instruction=EOR, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="EOR"}, 
    [0x5D] = {.instruction=EOR, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="EOR"}, // cyc+1 if page crossed
    [0x59] = {.instruction=EOR, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="EOR"}, // cyc+1 if page crossed
    [0x41] = {.instruction=EOR, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="EOR"}, 
    [0x51] = {.instruction=EOR, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="EOR"}, // cyc+1 if page crossed 
    // Flag (Processor Status) Instructions
    [0x18] = {.instruction=CLC, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="CLC"}, 
    [0x38] = {.instruction=SEC, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="SEC"}, 
    [0x58] = {.instruction=CLI, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="CLI"}, 
    [0x78] = {.instruction=SEI, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="SEI"}, 
    [0xB8] = {.instruction=CLV, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="CLV"}, 
    [0xD8] = {.instruction=CLD, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="CLD"}, 
    [0xF8] = {.instruction=SED, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="SED"}, 
    // INC (INCrement memory)
    // Affects Flags: N Z
    [0xE6] = {.instruction=INC, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=5, .name="INC"}, 
    [0xF6] = {.instruction=INC, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=6, .name="INC"}, 
    [0xEE] = {.instruction=INC, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="INC"}, 
    [0xFE] = {.instruction=INC, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=7, .name="INC"}, 
    // JMP (JuMP)
    // Affects Flags: none 
    [0x4C] = {.instruction=JMP, .addrmode=addressmode_absolute,   .length=3, .base_cycles=3, .name="JMP"}, 
    [0x6C] = {.instruction=JMP, .addrmode=addressmode_indirect,   .length=3, .base_cycles=5, .name="JMP"}, 
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
    [0x20] = {.instruction=JSR, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="JSR"}, 
    // LDA (LoaD Accumulator)
    // Affects Flags: N Z
    [0xA9] = {.instruction=LDA, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="LDA"}, 
    [0xA5] = {.instruction=LDA, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="LDA"}, 
    [0xB5] = {.instruction=LDA, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="LDA"}, 
    [0xAD] = {.instruction=LDA, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="LDA"}, 
    [0xBD] = {.instruction=LDA, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="LDA"}, // cyc+1 if page crossed 
    [0xB9] = {.instruction=LDA, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="LDA"}, // cyc+1 if page crossed 
    [0xA1] = {.instruction=LDA, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="LDA"}, 
    [0xB1] = {.instruction=LDA, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="LDA"}, // cyc+1 if page crossed 
    // LDX (LoaD X register)
    // Affects Flags: N Z
    [0xA2] = {.instruction=LDX, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="LDX"}, 
    [0xA6] = {.instruction=LDX, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="LDX"}, 
    [0xB6] = {.instruction=LDX, .addrmode=addressmode_zeropage_y, .length=2, .base_cycles=4, .name="LDX"}, 
    [0xAE] = {.instruction=LDX, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="LDX"}, 
    [0xBE] = {.instruction=LDX, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="LDX"}, // cyc+1 if page crossed 
    // LDY (LoaD Y register)
    // Affects Flags: N Z
    [0xA0] = {.instruction=LDY, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="LDY"}, 
    [0xA4] = {.instruction=LDY, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="LDY"}, 
    [0xB4] = {.instruction=LDY, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="LDY"}, 
    [0xAC] = {.instruction=LDY, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="LDY"}, 
    [0xBC] = {.instruction=LDY, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="LDY"}, // cyc+1 if page crossed 
    // LSR (Logical Shift Right)
    // Affects Flags: N Z C
    [0x4A] = {.instruction=LSR, .addrmode=addressmode_accumulator,.length=1, .base_cycles=2, .name="LSR"}, // LSR A 
    [0x46] = {.instruction=LSR, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=5, .name="LSR"}, 
    [0x56] = {.instruction=LSR, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=6, .name="LSR"}, 
    [0x4E] = {.instruction=LSR, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="LSR"}, 
    [0x5E] = {.instruction=LSR, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=7, .name="LSR"}, 
    // NOP (No OPeration)
    [0xEA] = {.instruction=NOP, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="NOP"}, 
    // ORA (bitwise OR with Accumulator)
    // Affects Flags: N Z
    [0x09] = {.instruction=ORA, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="ORA"}, 
    [0x05] = {.instruction=ORA, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="ORA"}, 
    [0x15] = {.instruction=ORA, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="ORA"}, 
    [0x0D] = {.instruction=ORA, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="ORA"}, 
    [0x1D] = {.instruction=ORA, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="ORA"}, // cyc+1 if page crossed
    [0x19] = {.instruction=ORA, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="ORA"}, // cyc+1 if page crossed 
    [0x01] = {.instruction=ORA, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="ORA"}, 
    [0x11] = {.instruction=ORA, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="ORA"}, // cyc+1 if page crossed 
    // Register Instructions
    // Affect Flags: N Z
    [0xAA] = {.instruction=TAX, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="TAX"}, 
    [0x8A] = {.instruction=TXA, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="TXA"}, 
    [0xCA] = {.instruction=DEX, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="DEX"}, 
    [0xE8] = {.instruction=INX, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="INX"}, 
    [0xA8] = {.instruction=TAY, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="TAY"}, 
    [0x98] = {.instruction=TYA, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="TYA"}, 
    [0x88] = {.instruction=DEY, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="DEY"}, 
    [0xC8] = {.instruction=INY, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="INY"}, 
    // ROL (ROtate Left)
    // Affects Flags: N Z C
    [0x2A] = {.instruction=ROL, .addrmode=addressmode_accumulator,.length=1, .base_cycles=2, .name="ROL"}, 
    [0x26] = {.instruction=ROL, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=5, .name="ROL"}, 
    [0x36] = {.instruction=ROL, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=6, .name="ROL"}, 
    [0x2E] = {.instruction=ROL, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="ROL"}, 
    [0x3E] = {.instruction=ROL, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=7, .name="ROL"}, 
    // ROR (ROtate Right)
    // Affects Flags: N Z C
    [0x6A] = {.instruction=ROR, .addrmode=addressmode_accumulator,.length=1, .base_cycles=2, .name="ROR"}, 
    [0x66] = {.instruction=ROR, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=5, .name="ROR"}, 
    [0x76] = {.instruction=ROR, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=6, .name="ROR"}, 
    [0x6E] = {.instruction=ROR, .addrmode=addressmode_absolute,   .length=3, .base_cycles=6, .name="ROR"}, 
    [0x7E] = {.instruction=ROR, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=7, .name="ROR"}, 
    // RTI (ReTurn from Interrupt)
    // Affects Flags: all
    [0x40] = {.instruction=RTI, .addrmode=addressmode_implicit,   .length=1, .base_cycles=6, .name="RTI"}, 
    // RTS (ReTurn from Subroutine)
    // Affects Flags: none
    [0x60] = {.instruction=RTS, .addrmode=addressmode_implicit,   .length=1, .base_cycles=6, .name="RTS"}, 
    // SBC (SuBtract with Carry)
    // Affects Flags: N V Z C
    [0xE9] = {.instruction=SBC, .addrmode=addressmode_immediate,  .length=2, .base_cycles=2, .name="SBC"}, 
    [0xE5] = {.instruction=SBC, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="SBC"}, 
    [0xF5] = {.instruction=SBC, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="SBC"}, 
    [0xED] = {.instruction=SBC, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="SBC"}, 
    [0xFD] = {.instruction=SBC, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=4, .name="SBC"}, // cyc+1 if page crossed
    [0xF9] = {.instruction=SBC, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=4, .name="SBC"}, // cyc+1 if page crossed 
    [0xE1] = {.instruction=SBC, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="SBC"}, 
    [0xF1] = {.instruction=SBC, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=5, .name="SBC"}, // cyc+1 if page crossed 
    // STA (STore Accumulator)
    // Affects Flags: none
    [0x85] = {.instruction=STA, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="STA"}, 
    [0x95] = {.instruction=STA, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="STA"}, 
    [0x8D] = {.instruction=STA, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="STA"}, 
    [0x9D] = {.instruction=STA, .addrmode=addressmode_absolute_x, .length=3, .base_cycles=5, .name="STA"}, 
    [0x99] = {.instruction=STA, .addrmode=addressmode_absolute_y, .length=3, .base_cycles=5, .name="STA"}, 
    [0x81] = {.instruction=STA, .addrmode=addressmode_indirect_x, .length=2, .base_cycles=6, .name="STA"}, 
    [0x91] = {.instruction=STA, .addrmode=addressmode_indirect_y, .length=2, .base_cycles=6, .name="STA"}, 
    // Stack Instructions
    [0x9A] = {.instruction=TXS, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="TXS"}, 
    [0xBA] = {.instruction=TSX, .addrmode=addressmode_implicit,   .length=1, .base_cycles=2, .name="TSX"}, 
    [0x48] = {.instruction=PHA, .addrmode=addressmode_implicit,   .length=1, .base_cycles=3, .name="PHA"}, 
    [0x68] = {.instruction=PLA, .addrmode=addressmode_implicit,   .length=1, .base_cycles=4, .name="PLA"}, 
    [0x08] = {.instruction=PHP, .addrmode=addressmode_implicit,   .length=1, .base_cycles=3, .name="PHP"},
    [0x28] = {.instruction=PLP, .addrmode=addressmode_implicit,   .length=1, .base_cycles=4, .name="PLP"},  
    // STX (STore X register)
    // Affects Flags: none
    [0x86] = {.instruction=STX, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="STX"}, 
    [0x96] = {.instruction=STX, .addrmode=addressmode_zeropage_y, .length=2, .base_cycles=4, .name="STX"}, 
    [0x8E] = {.instruction=STX, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="STX"}, 
    // STY (STore Y register)
    // Affects Flags: none
    [0x84] = {.instruction=STY, .addrmode=addressmode_zeropage,   .length=2, .base_cycles=3, .name="STY"}, 
    [0x94] = {.instruction=STY, .addrmode=addressmode_zeropage_x, .length=2, .base_cycles=4, .name="STY"}, 
    [0x8C] = {.instruction=STY, .addrmode=addressmode_absolute,   .length=3, .base_cycles=4, .name="STY"}, 
};
