#include "cpu.h"
#include "cpu_opcodes.h"

void cpu_init(struct nescpu *cpu) {
    cpu->a = 0; 
    cpu->x = 0; 
    cpu->y = 0; 
    cpu->sp = CPU_SP_RESET; 
    cpu->p = (uint8_t)cpu_flag_break2 | (uint8_t)cpu_flag_interrupt_disable; 
    cpu->pc = 0; 
}

const enum addressmode type1_bbb_to_addrmode[8] = {
    [0b000] = addressmode_indirect_x, // len=2, cyc=6
    [0b001] = addressmode_zeropage,   // len=2, cyc=3
    [0b010] = addressmode_immediate,  // len=2, cyc=2
    [0b011] = addressmode_absolute,   // len=3, cyc=4
    [0b100] = addressmode_indirect_y, // len=2, cyc=5+1 (STA: always+1, others+1 if page crossed)
    [0b101] = addressmode_zeropage_x, // len=2, cyc=4
    [0b110] = addressmode_absolute_y, // len=3, cyc=4+1 (STA: always+1, others+1 if page crossed)
    [0b111] = addressmode_absolute_x, // len=3, cyc=4+1 (STA: always+1, others+1 if page crossed)
};

static void execute_type1_ALU_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    switch (aaa) {
        case 0b000: 
            // ORA
            break;
        case 0b001: 
            // AND
            break;
        case 0b010: 
            // EOR 
            break;
        case 0b011: 
            // ADC 
            break;
        case 0b100: 
            // STA 
            break;
        case 0b101: 
            // LDA
            break;
        case 0b110: 
            // CMP
            break;
        case 0b111: 
            // SBC 
            break; 
        default: 
            panic("invalid aaa %u\n", aaa); 
            break; 
    }
}
static void execute_type2_RMW_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb); 
static void execute_type0_control_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb); 
static void execute_type3_unofficial_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb); 

void cpu_run(struct nescpu *cpu, void(*callback)(struct nescpu *c)) {
    while (1) {
        if (callback) {
            callback(cpu); 
        }
        uint8_t opcode = cpu_read8(cpu, cpu->pc); 
        const struct cpu_opcode *opcode_entry = &(opcode_table[opcode]);  
        cpu->pc += 1; 
        // Most instructions that explicitly reference memory locations have bit patterns of the form aaabbbcc. 
        // The aaa and cc bits determine the opcode, and the bbb bits determine the addressing mode.
        unsigned aaa = (opcode >> 5);
        unsigned bbb = (opcode >> 2) & 0b111; 
        unsigned cc = (opcode & 0b11); 

        switch (cc) {
            case 0b01: 
                // type 1 (ALU)
                execute_type1_ALU_instruction(cpu, opcode_entry, aaa, bbb); 
                break;
            case 0b10: 
                // type 2 (RMW)
                execute_type2_RMW_instruction(cpu, opcode_entry, aaa, bbb); 
                break;
            case 0b00: 
                // type 0 (control)
                execute_type0_control_instruction(cpu, opcode_entry, aaa, bbb); 
                break;
            case 0b11: 
                // type 3 unofficial 
                execute_type3_unofficial_instruction(cpu, opcode_entry, aaa, bbb); 
                break;
            default: 
                panic("illegal opcode: %u\n", (unsigned)opcode);
                break; 
        }
    }
}