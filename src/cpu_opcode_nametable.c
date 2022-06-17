#include "cpu_opcodes.h"

/**
 * How to generate the following table: 
 * To generate one row of the table: 
 * 1. Go to https://www.nesdev.org/wiki/CPU_unofficial_opcodes ...
 * 2. Copy one row of the first colorful table from the above webpage ...
 * 3. Paste it into the array ...
 * 4. On vscode: search for regular expression .*\t and replace it with empty string, this trims the addressmode information ...
 * 5. Search for regular expression ([A-Z]{3})\n this will highlight the three-letter instruction names ...
 * 6. replace the above with "$1",
 * Now you have one single row! ...
 * Then do the other rows ...
 * You can also slightly fix the rows to make it a bit shorter, like this ...
 */

// note: this is a table having 256 entries with 4 bytes each. 
const char opcode_nametable[256][4] = {
    "BRK","ORA","STP","SLO","NOP","ORA","ASL","SLO","PHP","ORA","ASL","ANC","NOP","ORA","ASL","SLO",//0x00+
    "BPL","ORA","STP","SLO","NOP","ORA","ASL","SLO","CLC","ORA","NOP","SLO","NOP","ORA","ASL","SLO",//0x10+
    "JSR","AND","STP","RLA","BIT","AND","ROL","RLA","PLP","AND","ROL","ANC","BIT","AND","ROL","RLA",//0x20+
    "BMI","AND","STP","RLA","NOP","AND","ROL","RLA","SEC","AND","NOP","RLA","NOP","AND","ROL","RLA",//0x30+
    "RTI","EOR","STP","SRE","NOP","EOR","LSR","SRE","PHA","EOR","LSR","ALR","JMP","EOR","LSR","SRE",//0x40+
    "BVC","EOR","STP","SRE","NOP","EOR","LSR","SRE","CLI","EOR","NOP","SRE","NOP","EOR","LSR","SRE",//0x50+
    "RTS","ADC","STP","RRA","NOP","ADC","ROR","RRA","PLA","ADC","ROR","ARR","JMP","ADC","ROR","RRA",//0x60+
    "BVS","ADC","STP","RRA","NOP","ADC","ROR","RRA","SEI","ADC","NOP","RRA","NOP","ADC","ROR","RRA",//0x70+
    "NOP","STA","NOP","SAX","STY","STA","STX","SAX","DEY","NOP","TXA","XAA","STY","STA","STX","SAX",//0x80+
    "BCC","STA","STP","AHX","STY","STA","STX","SAX","TYA","STA","TXS","TAS","SHY","STA","SHX","AHX",//0x90+
    "LDY","LDA","LDX","LAX","LDY","LDA","LDX","LAX","TAY","LDA","TAX","LAX","LDY","LDA","LDX","LAX",//0xA0+
    "BCS","LDA","STP","LAX","LDY","LDA","LDX","LAX","CLV","LDA","TSX","LAS","LDY","LDA","LDX","LAX",//0xB0+
    "CPY","CMP","NOP","DCP","CPY","CMP","DEC","DCP","INY","CMP","DEX","AXS","CPY","CMP","DEC","DCP",//0xC0+
    "BNE","CMP","STP","DCP","NOP","CMP","DEC","DCP","CLD","CMP","NOP","DCP","NOP","CMP","DEC","DCP",//0xD0+
    "CPX","SBC","NOP","ISC","CPX","SBC","INC","ISC","INX","SBC","NOP","SBC","CPX","SBC","INC","ISC",//0xE0+
    "BEQ","SBC","STP","ISC","NOP","SBC","INC","ISC","SED","SBC","NOP","ISC","NOP","SBC","INC","ISC",//0xF0+

};

void print_cpu_opcode_nametable(void) {
    printf("const char opcode_nametable[256][4] = {\n");
    for (size_t i = 0x00; i <= 0xFF; ++i) {
        if ((i & 0x1F) == 0) printf("    ");
        printf("\"%s\",", opcode_nametable[i]);
        if ((i & 0x1F) == 0x1F) printf("\n");
    }
    printf("};\n");
}
