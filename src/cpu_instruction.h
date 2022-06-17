#pragma once

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

#define OP_INSTR_0x00 BRK
#define OP_INSTR_0x20 JSR
#define OP_INSTR_0x40 RTI
#define OP_INSTR_0x60 RTS
#define OP_INSTR_0x80 NOP_
#define OP_INSTR_0xA0 LDY
#define OP_INSTR_0xC0 CPY
#define OP_INSTR_0xE0 CPX

#define OP_INSTR_0x04 NOP_
#define OP_INSTR_0x24 BIT
#define OP_INSTR_0x44 NOP_
#define OP_INSTR_0x64 NOP_
#define OP_INSTR_0x84 STY
#define OP_INSTR_0xA4 LDY
#define OP_INSTR_0xC4 CPY
#define OP_INSTR_0xE4 CPX

#define OP_INSTR_0x08 PHP
#define OP_INSTR_0x28 PLP
#define OP_INSTR_0x48 PHA
#define OP_INSTR_0x68 PLA
#define OP_INSTR_0x88 DEY
#define OP_INSTR_0xA8 TAY
#define OP_INSTR_0xC8 INY
#define OP_INSTR_0xE8 INX

#define OP_INSTR_0x0C NOP_
#define OP_INSTR_0x2C BIT
#define OP_INSTR_0x4C JMP
#define OP_INSTR_0x6C JMP
#define OP_INSTR_0x8C STY
#define OP_INSTR_0xAC LDY
#define OP_INSTR_0xCC CPY
#define OP_INSTR_0xEC CPX

#define OP_INSTR_0x10 BPL
#define OP_INSTR_0x30 BMI
#define OP_INSTR_0x50 BVC
#define OP_INSTR_0x70 BVS
#define OP_INSTR_0x90 BCC
#define OP_INSTR_0xB0 BCS
#define OP_INSTR_0xD0 BNE
#define OP_INSTR_0xF0 BEQ

#define OP_INSTR_0x14 NOP_
#define OP_INSTR_0x34 NOP_
#define OP_INSTR_0x54 NOP_
#define OP_INSTR_0x74 NOP_
#define OP_INSTR_0x94 STY
#define OP_INSTR_0xB4 LDY
#define OP_INSTR_0xD4 NOP_
#define OP_INSTR_0xF4 NOP_

#define OP_INSTR_0x18 CLC
#define OP_INSTR_0x38 SEC
#define OP_INSTR_0x58 CLI
#define OP_INSTR_0x78 SEI
#define OP_INSTR_0x98 TYA
#define OP_INSTR_0xB8 CLV
#define OP_INSTR_0xD8 CLD
#define OP_INSTR_0xF8 SED

#define OP_INSTR_0x1C NOP_
#define OP_INSTR_0x3C NOP_
#define OP_INSTR_0x5C NOP_
#define OP_INSTR_0x7C NOP_
#define OP_INSTR_0x9C SHY_
#define OP_INSTR_0xBC LDY
#define OP_INSTR_0xDC NOP_
#define OP_INSTR_0xFC NOP_

///////////////////////////////////////////////////////////
#define OP_INSTR_0x01 ORA
#define OP_INSTR_0x21 AND
#define OP_INSTR_0x41 EOR
#define OP_INSTR_0x61 ADC
#define OP_INSTR_0x81 STA
#define OP_INSTR_0xA1 LDA
#define OP_INSTR_0xC1 CMP
#define OP_INSTR_0xE1 SBC

#define OP_INSTR_0x05 ORA
#define OP_INSTR_0x25 AND
#define OP_INSTR_0x45 EOR
#define OP_INSTR_0x65 ADC
#define OP_INSTR_0x85 STA
#define OP_INSTR_0xA5 LDA
#define OP_INSTR_0xC5 CMP
#define OP_INSTR_0xE5 SBC

#define OP_INSTR_0x09 ORA
#define OP_INSTR_0x29 AND
#define OP_INSTR_0x49 EOR
#define OP_INSTR_0x69 ADC
#define OP_INSTR_0x89 NOP_
#define OP_INSTR_0xA9 LDA
#define OP_INSTR_0xC9 CMP
#define OP_INSTR_0xE9 SBC

#define OP_INSTR_0x0D ORA
#define OP_INSTR_0x2D AND
#define OP_INSTR_0x4D EOR
#define OP_INSTR_0x6D ADC
#define OP_INSTR_0x8D STA
#define OP_INSTR_0xAD LDA
#define OP_INSTR_0xCD CMP
#define OP_INSTR_0xED SBC

#define OP_INSTR_0x11 ORA
#define OP_INSTR_0x31 AND
#define OP_INSTR_0x51 EOR
#define OP_INSTR_0x71 ADC
#define OP_INSTR_0x91 STA
#define OP_INSTR_0xB1 LDA
#define OP_INSTR_0xD1 CMP
#define OP_INSTR_0xF1 SBC

#define OP_INSTR_0x15 ORA
#define OP_INSTR_0x35 AND
#define OP_INSTR_0x55 EOR
#define OP_INSTR_0x75 ADC
#define OP_INSTR_0x95 STA
#define OP_INSTR_0xB5 LDA
#define OP_INSTR_0xD5 CMP
#define OP_INSTR_0xF5 SBC

#define OP_INSTR_0x19 ORA
#define OP_INSTR_0x39 AND
#define OP_INSTR_0x59 EOR
#define OP_INSTR_0x79 ADC
#define OP_INSTR_0x99 STA
#define OP_INSTR_0xB9 LDA
#define OP_INSTR_0xD9 CMP
#define OP_INSTR_0xF9 SBC

#define OP_INSTR_0x1D ORA
#define OP_INSTR_0x3D AND
#define OP_INSTR_0x5D EOR
#define OP_INSTR_0x7D ADC
#define OP_INSTR_0x9D STA
#define OP_INSTR_0xBD LDA
#define OP_INSTR_0xDD CMP
#define OP_INSTR_0xFD SBC

///////////////////////////////////////////////////////////
#define OP_INSTR_0x02 STP_
#define OP_INSTR_0x22 STP_
#define OP_INSTR_0x42 STP_
#define OP_INSTR_0x62 STP_
#define OP_INSTR_0x82 NOP_
#define OP_INSTR_0xA2 LDX
#define OP_INSTR_0xC2 NOP_
#define OP_INSTR_0xE2 NOP_

#define OP_INSTR_0x06 ASL
#define OP_INSTR_0x26 ROL
#define OP_INSTR_0x46 LSR
#define OP_INSTR_0x66 ROR
#define OP_INSTR_0x86 STX
#define OP_INSTR_0xA6 LDX
#define OP_INSTR_0xC6 DEC
#define OP_INSTR_0xE6 INC

#define OP_INSTR_0x0A ASL_A
#define OP_INSTR_0x2A ROL_A
#define OP_INSTR_0x4A LSR_A
#define OP_INSTR_0x6A ROR_A
#define OP_INSTR_0x8A TXA
#define OP_INSTR_0xAA TAX
#define OP_INSTR_0xCA DEX
#define OP_INSTR_0xEA NOP

#define OP_INSTR_0x0E ASL
#define OP_INSTR_0x2E ROL
#define OP_INSTR_0x4E LSR
#define OP_INSTR_0x6E ROR
#define OP_INSTR_0x8E STX
#define OP_INSTR_0xAE LDX
#define OP_INSTR_0xCE DEC
#define OP_INSTR_0xEE INC

#define OP_INSTR_0x12 STP_
#define OP_INSTR_0x32 STP_
#define OP_INSTR_0x52 STP_
#define OP_INSTR_0x72 STP_
#define OP_INSTR_0x92 STP_
#define OP_INSTR_0xB2 STP_
#define OP_INSTR_0xD2 STP_
#define OP_INSTR_0xF2 STP_

#define OP_INSTR_0x16 ASL
#define OP_INSTR_0x36 ROL
#define OP_INSTR_0x56 LSR
#define OP_INSTR_0x76 ROR
#define OP_INSTR_0x96 STX
#define OP_INSTR_0xB6 LDX
#define OP_INSTR_0xD6 DEC
#define OP_INSTR_0xF6 INC

#define OP_INSTR_0x1A NOP_
#define OP_INSTR_0x3A NOP_
#define OP_INSTR_0x5A NOP_
#define OP_INSTR_0x7A NOP_
#define OP_INSTR_0x9A TXS
#define OP_INSTR_0xBA TSX
#define OP_INSTR_0xDA NOP_
#define OP_INSTR_0xFA NOP_

#define OP_INSTR_0x1E ASL
#define OP_INSTR_0x3E ROL
#define OP_INSTR_0x5E LSR
#define OP_INSTR_0x7E ROR
#define OP_INSTR_0x9E SHX_
#define OP_INSTR_0xBE LDX
#define OP_INSTR_0xDE DEC
#define OP_INSTR_0xFE INC

///////////////////////////////////////////////////////////
#define OP_INSTR_0x03 SLO_
#define OP_INSTR_0x23 RLA_
#define OP_INSTR_0x43 SRE_
#define OP_INSTR_0x63 RRA_
#define OP_INSTR_0x83 SAX_
#define OP_INSTR_0xA3 LAX_
#define OP_INSTR_0xC3 DCP_
#define OP_INSTR_0xE3 ISC_

#define OP_INSTR_0x07 SLO_
#define OP_INSTR_0x27 RLA_
#define OP_INSTR_0x47 SRE_
#define OP_INSTR_0x67 RRA_
#define OP_INSTR_0x87 SAX_
#define OP_INSTR_0xA7 LAX_
#define OP_INSTR_0xC7 DCP_
#define OP_INSTR_0xE7 ISC_

#define OP_INSTR_0x0B ANC_
#define OP_INSTR_0x2B ANC_
#define OP_INSTR_0x4B ALR_
#define OP_INSTR_0x6B ARR_
#define OP_INSTR_0x8B XAA_
#define OP_INSTR_0xAB LAX_
#define OP_INSTR_0xCB AXS_
#define OP_INSTR_0xEB SBC_

#define OP_INSTR_0x0F SLO_
#define OP_INSTR_0x2F RLA_
#define OP_INSTR_0x4F SRE_
#define OP_INSTR_0x6F RRA_
#define OP_INSTR_0x8F SAX_
#define OP_INSTR_0xAF LAX_
#define OP_INSTR_0xCF DCP_
#define OP_INSTR_0xEF ISC_

#define OP_INSTR_0x13 SLO_
#define OP_INSTR_0x33 RLA_
#define OP_INSTR_0x53 SRE_
#define OP_INSTR_0x73 RRA_
#define OP_INSTR_0x93 AHX_
#define OP_INSTR_0xB3 LAX_
#define OP_INSTR_0xD3 DCP_
#define OP_INSTR_0xF3 ISC_

#define OP_INSTR_0x17 SLO_
#define OP_INSTR_0x37 RLA_
#define OP_INSTR_0x57 SRE_
#define OP_INSTR_0x77 RRA_
#define OP_INSTR_0x97 SAX_
#define OP_INSTR_0xB7 LAX_
#define OP_INSTR_0xD7 DCP_
#define OP_INSTR_0xF7 ISC_

#define OP_INSTR_0x1B SLO_
#define OP_INSTR_0x3B RLA_
#define OP_INSTR_0x5B SRE_
#define OP_INSTR_0x7B RRA_
#define OP_INSTR_0x9B TAS_
#define OP_INSTR_0xBB LAS_
#define OP_INSTR_0xDB DCP_
#define OP_INSTR_0xFB ISC_

#define OP_INSTR_0x1F SLO_
#define OP_INSTR_0x3F RLA_
#define OP_INSTR_0x5F SRE_
#define OP_INSTR_0x7F RRA_
#define OP_INSTR_0x9F AHX_
#define OP_INSTR_0xBF LAX_
#define OP_INSTR_0xDF DCP_
#define OP_INSTR_0xFF ISC_


#define OP_GETINSTR(...) OP_GETINSTR_HEX(__VA_ARGS__)
#define OP_GETINSTR_HEX(OPCODE_HEX) OP_INSTR_##OPCODE_HEX


