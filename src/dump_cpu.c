#include "cpu.h"
#include "cpu_addressmode.h"
#include "cpu_instruction.h"

static inline void print_instr(struct nescpu *cpu, uint8_t opcode, enum cpu_instruction instr, unsigned len, enum addressmode addrmode, char str[8]) {
    uint16_t literal_address = cpu_read8(cpu, cpu->pc+1); 
    printf("%04X  ", cpu->pc);
    uint8_t instr_hex[3] = {0, 0, 0}; 
    for (unsigned i = 0; i < 3; ++i) {
        if (i >= len) {
            printf("   "); 
        } else {
            instr_hex[i] = cpu_read8(cpu, cpu->pc + i); 
            printf("%02X ", instr_hex[i]); 
        }
    }
    str[4] = '\0';
    str[3] = '\0';
    if (instr >= _UNOFFICIAL_INSTRUCTION_START) {
        if (instr == ISC_) {
            str[2] = 'B';  // change ISC to ISB, they are the same but with different representation name
        }
        printf("*%s ", str); 
    } else {
        printf(" %s ", str); 
    }
    bool ispagecrossed = false;
    uint16_t mem_addr = cpu_compute_address(cpu, addrmode, cpu->pc + 1, &ispagecrossed); 
    int cprinted = 0; 
    switch (addrmode) {
        case MODE_IMM: 
            cprinted += printf("#$%02X", literal_address); 
            break; 
        case MODE_ZPG: 
            cprinted += printf("$%02X = %02X", mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case MODE_ZPX: 
            cprinted += printf("$%02X,X @ %02X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case MODE_ZPY: 
            cprinted += printf("$%02X,Y @ %02X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case MODE_ABS: 
            if (opcode == 0x4c || opcode == 0x20) {
                // JMP absolute or JSR absolute 
                cprinted += printf("$%04X", cpu_read16(cpu, cpu->pc+1)); 
            } else {
                cprinted += printf("$%04X = %02X", mem_addr, cpu_read8(cpu, mem_addr));
            }
            break; 
        case MODE_ABX: 
            literal_address = cpu_read16(cpu, cpu->pc + 1); 
            cprinted += printf("$%04X,X @ %04X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case MODE_ABY: 
            literal_address = cpu_read16(cpu, cpu->pc + 1); 
            cprinted += printf("$%04X,Y @ %04X = %02X", literal_address, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case MODE_INX: 
            cprinted += printf("($%02X,X) @ %02X = %04X = %02X", literal_address, (literal_address+cpu->x)&0xFF, mem_addr, cpu_read8(cpu, mem_addr)); 
            break; 
        case MODE_INY: 
            cprinted += printf("($%02X),Y = %04X @ %04X = %02X", literal_address, (mem_addr-(uint16_t)cpu->y)&0xFFFF, mem_addr, cpu_read8(cpu, mem_addr));
            break;  
        case MODE_IMP: 
            // cprinted += printf("                            ");
            if (opcode == 0x0a || opcode == 0x4a || opcode == 0x2a || opcode == 0x6a) {
                cprinted += printf("A "); 
            }
            break; 
        case MODE_REL: 
            // branches 
            cprinted += printf("$%04X", ((int16_t)cpu->pc + 2 + (int16_t)((int8_t)literal_address)) & 0xFFFF);
            break; 
        case MODE_IND: 
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

#define OPCODE_DUMP(OP) \
    case OP: {\
        char str[8] = INSTR_TO_STR(OP_GETINSTR(OP));\
        print_instr(cpu, OP, OP_GETINSTR(OP), OP_GETLENGTH(OP), OP_GETMODE(OP), str); \
    break;}
void dump_cpu(struct nescpu *cpu) {
    uint8_t opcode = cpu_read8(cpu, cpu->pc);
    switch (opcode) {
        OPCODE_CASES(OPCODE_DUMP)
        default: panic("unknown opcode %02X\n", opcode);
    }
}
