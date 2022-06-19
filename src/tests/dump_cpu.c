#include "cpu.h"
#include "cpu_opcodes.h"
// #include "cpu_addressmode.h"
void dump_cpu(struct nescpu *cpu) {
    uint8_t opcode = cpu_read8(cpu, cpu->pc); 
    uint16_t literal_address = cpu_read8(cpu, cpu->pc+1); 
    struct cpu_opcode op = opcodes[opcode]; 
    unsigned len = op.length; 
    printf("%04X  ", cpu->pc);
    if (cpu->pc == 0xCDAE) {
        // printf("%02X %02X %02X %02X\n", cpu_read8(cpu, 0xCDAE),cpu_read8(cpu, 0xCDAE +1),cpu_read8(cpu, 0xCDAE +2),cpu_read8(cpu, 0xCDAE +3));
    }
    uint8_t instr_hex[3] = {0, 0, 0}; 
    for (unsigned i = 0; i < 3; ++i) {
        if (i >= len) {
            printf("   "); 
        } else {
            instr_hex[i] = cpu_read8(cpu, cpu->pc + i); 
            printf("%02X ", instr_hex[i]); 
        }
    }
    if (op.name[0] == '*') {
        printf("%s ", op.name); 
    } else {
        printf(" %s ", op.name); 
    }
    bool ispagecrossed = false;
    uint16_t mem_addr = cpu_compute_address(cpu, op.addrmode, cpu->pc + 1, &ispagecrossed); 
    int cprinted = 0; 
    switch (op.addrmode) {
        case addressmode_imm: 
            cprinted += printf("#$%02X", literal_address); 
            break; 
        case addressmode_zeropage: 
            cprinted += printf("$%02X = %02X", mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case addressmode_zeropage_x: 
            cprinted += printf("$%02X,X @ %02X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case addressmode_zeropage_y: 
            cprinted += printf("$%02X,Y @ %02X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case addressmode_absolute: 
            if (opcode == 0x4c || opcode == 0x20) {
                // JMP absolute or JSR absolute 
                cprinted += printf("$%04X", cpu_read16(cpu, cpu->pc+1)); 
            } else {
                cprinted += printf("$%04X = %02X", mem_addr, cpu_read8(cpu, mem_addr));
            }
            break; 
        case addressmode_absolute_x: 
            literal_address = cpu_read16(cpu, cpu->pc + 1); 
            cprinted += printf("$%04X,X @ %04X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case addressmode_absolute_y: 
            literal_address = cpu_read16(cpu, cpu->pc + 1); 
            cprinted += printf("$%04X,Y @ %04X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case addressmode_indirect_x: 
            cprinted += printf("($%02X,X) @ %02X = %04X = %02X", literal_address, (literal_address+cpu->x)&0xFF, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case addressmode_indirect_y: 
            cprinted += printf("($%02X),Y = %04X @ %04X = %02X", literal_address, (mem_addr-(uint16_t)cpu->y)&0xFFFF, mem_addr, cpu_read8(cpu, mem_addr));
            break;  
        case addressmode_none: 
            // cprinted += printf("                            ");
            if (opcode == 0x0a || opcode == 0x4a || opcode == 0x2a || opcode == 0x6a) {
                cprinted += printf("A "); 
            }
            break; 
        case addressmode_relative: 
            // branches 
            cprinted += printf("$%04X", ((int16_t)cpu->pc + 2 + (int16_t)((int8_t)literal_address)) & 0xFFFF);
            break; 
        case addressmode_indirect: 
            // jump indirect 
            {
                uint16_t jmp_addr = 0; 
                literal_address = cpu_read16(cpu, cpu->pc + 1); 
                if ((literal_address & 0x00FF) == 0xFF) {
                    uint16_t lo = cpu_read8(cpu, literal_address); 
                    uint16_t hi = cpu_read8(cpu, literal_address & 0xFF00); 
                    jmp_addr = (hi << 8) | lo; 
                } else {
                    jmp_addr = cpu_read16(cpu, literal_address); 
                }
                cprinted += printf("($%04X) = %04X", literal_address, jmp_addr); 
            }
            break; 
        default: 
            cprinted += printf("???");
            break; 
    }
    while (cprinted < 28) {
        cprinted += printf(" "); 
    }
    printf("A:%02X X:%02X Y:%02X P:%02X SP:%02X", cpu->a, cpu->x, cpu->y, cpu->p, cpu->sp); 

    printf("\n"); 

}