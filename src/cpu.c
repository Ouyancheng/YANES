/**
 * The NES's 6502 CPU 
 * Very useful information: 
 * Opcode description: https://llx.com/Neil/a2/opcodes.html 
 * The 6502 instruction set: https://www.masswerk.at/6502/6502_instruction_set.html 
 * The illegal opcodes: https://www.masswerk.at/nowgobang/2021/6502-illegal-opcodes 
 * The opcode table: https://www.nesdev.org/wiki/CPU_unofficial_opcodes 
 * The 6502 simulator: http://www.visual6502.org/JSSim/expert.html 
 * The instructions: http://www.6502.org/tutorials/6502opcodes.html 
 */
#include "cpu.h"
#include "cpu_opcodes.h"
#define internal_function static inline
internal_function void set_flag_carry(struct nescpu *cpu, bool val) { 
    // cpu->p = (val) ? (cpu->p | (uint8_t)cpu_flag_carry) : (cpu->p & (~(uint8_t)cpu_flag_carry)); 
    cpu->flag_C = val; 
}
internal_function void set_flag_zero(struct nescpu *cpu, bool val) { 
    // cpu->p = (val) ? (cpu->p | (uint8_t)cpu_flag_zero) : (cpu->p & (~(uint8_t)cpu_flag_zero)); 
    cpu->flag_Z = val; 
}
internal_function void set_flag_IRQ(struct nescpu *cpu, bool val) { 
    // cpu->p = (val) ? (cpu->p | (uint8_t)cpu_flag_interrupt_disable) : (cpu->p & (~(uint8_t)cpu_flag_interrupt_disable)); 
    cpu->flag_I = val; 
}
internal_function void set_flag_decimal(struct nescpu *cpu, bool val) { 
    // cpu->p = (val) ? (cpu->p | (uint8_t)cpu_flag_decimal_mode) : (cpu->p & (~(uint8_t)cpu_flag_decimal_mode)); 
    cpu->flag_D = val; 
}
internal_function void set_flag_overflow(struct nescpu *cpu, bool val) { 
    // cpu->p = (val) ? (cpu->p | (uint8_t)cpu_flag_overflow) : (cpu->p & (~(uint8_t)cpu_flag_overflow)); 
    cpu->flag_V = val; 
}
internal_function void set_flag_negative(struct nescpu *cpu, bool val) { 
    // cpu->p = (val) ? (cpu->p | (uint8_t)cpu_flag_negative) : (cpu->p & (~(uint8_t)cpu_flag_negative)); 
    cpu->flag_N = val; 
}
internal_function bool get_flag_carry(struct nescpu *cpu) { 
    // return cpu->p & (uint8_t)cpu_flag_carry; 
    return cpu->flag_C; 
}
internal_function bool get_flag_zero(struct nescpu *cpu) { 
    // return cpu->p & (uint8_t)cpu_flag_zero; 
    return cpu->flag_Z; 
}
internal_function bool get_flag_IRQ(struct nescpu *cpu) { 
    // return cpu->p & (uint8_t)cpu_flag_interrupt_disable; 
    return cpu->flag_I; 
}
internal_function bool get_flag_decimal(struct nescpu *cpu) { 
    // return cpu->p & (uint8_t)cpu_flag_decimal_mode; 
    return cpu->flag_D; 
}
internal_function bool get_flag_overflow(struct nescpu *cpu) { 
    // return cpu->p & (uint8_t)cpu_flag_overflow; 
    return cpu->flag_V; 
}
internal_function bool get_flag_negative(struct nescpu *cpu) { 
    // return cpu->p & (uint8_t)cpu_flag_negative; 
    return cpu->flag_N; 
}
internal_function void set_flags_from_byte(struct nescpu *cpu, uint8_t p) {
    // cpu->p = p; 
    set_flag_carry(cpu, p & cpu_flag_carry); 
    set_flag_zero(cpu, p & cpu_flag_zero); 
    set_flag_IRQ(cpu, p & cpu_flag_interrupt_disable); 
    set_flag_decimal(cpu, p & cpu_flag_decimal_mode); 
    set_flag_overflow(cpu, p & cpu_flag_overflow); 
    set_flag_negative(cpu, p & cpu_flag_negative); 
}
internal_function uint8_t get_flags_to_byte(struct nescpu *cpu, bool flag_brk) {
    // return cpu->p; 
    return (
        ((uint8_t)get_flag_carry(cpu) << 0) | 
        ((uint8_t)get_flag_zero(cpu) << 1) | 
        ((uint8_t)get_flag_IRQ(cpu) << 2) | 
        ((uint8_t)get_flag_decimal(cpu) << 3) | 
        ((uint8_t)flag_brk << 4) | 
        (UINT8_C(1) << 5) | 
        ((uint8_t)get_flag_overflow(cpu) << 6) | 
        ((uint8_t)get_flag_negative(cpu) << 7)
    ); 
}
struct bbb_table_entry {
    enum addressmode mode; 
    unsigned length; 
    unsigned base_cycle;
};
const struct bbb_table_entry type1_bbb_lookup_table[8] = {
    [0b000] = {addressmode_indirect_x, 2, 6}, // len=2, cyc=6
    [0b001] = {addressmode_zeropage,   2, 3}, // len=2, cyc=3
    [0b010] = {addressmode_immediate,  2, 2}, // len=2, cyc=2
    [0b011] = {addressmode_absolute,   3, 4}, // len=3, cyc=4
    [0b100] = {addressmode_indirect_y, 2, 5}, // len=2, cyc=5+1 (STA: always+1, others+1 if page crossed)
    [0b101] = {addressmode_zeropage_x, 2, 4}, // len=2, cyc=4
    [0b110] = {addressmode_absolute_y, 3, 4}, // len=3, cyc=4+1 (STA: always+1, others+1 if page crossed)
    [0b111] = {addressmode_absolute_x, 3, 4}, // len=3, cyc=4+1 (STA: always+1, others+1 if page crossed)
};
const struct bbb_table_entry type2_bbb_lookup_table[8] = {
    [0b000] = {addressmode_immediate,  2, 2}, 
    [0b001] = {addressmode_zeropage,   2, 5},
    [0b010] = {addressmode_accumulator,1, 2}, 
    [0b011] = {addressmode_absolute,   3, 6}, 
    [0b100] = {addressmode_implicit,   1, 2}, // unofficial, the STP column, halts the cpu, reset required
    [0b101] = {addressmode_zeropage_x, 2, 6}, 
    [0b110] = {addressmode_implicit,   1, 2}, // unofficial NOPs, except for TXS and TSX 
    [0b111] = {addressmode_absolute_x, 3, 7}, 
};

internal_function void execute_type1_ALU_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    assert(0 <= aaa && aaa < 8); 
    assert(0 <= bbb && bbb < 8); 
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
internal_function void execute_type2_RMW_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    panic("TODO\n"); 
}
internal_function void execute_type0_control_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    panic("TODO\n"); 
}
internal_function void execute_type3_unofficial_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    panic("TODO: opcode aaa=%x bbb=%x cc=11 is an unofficial opcode\n", aaa, bbb); 
}

void cpu_run(struct nescpu *cpu, void(*callback)(struct nescpu *c)) {
    while (1) {
        if (callback) {
            callback(cpu); 
        }
        uint8_t opcode = cpu_read8(cpu, cpu->pc); 
        const struct cpu_opcode *opcode_entry = &(opcode_table[opcode]);  
        cpu->pc += 1; // increment the PC by 1 
        // Most instructions that explicitly reference memory locations have bit patterns of the form aaabbbcc. 
        // The aaa and cc bits determine the opcode, and the bbb bits determine the addressing mode.
        unsigned aaa = (opcode >> 5) & 0b111;
        unsigned bbb = (opcode >> 2) & 0b111; 
        unsigned cc  = (opcode >> 0) & 0b11; 

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

/// https://www.nesdev.org/wiki/CPU_power_up_state 
void cpu_init(struct nescpu *cpu) {
    cpu->a = 0; 
    cpu->x = 0; 
    cpu->y = 0; 
    cpu->sp = CPU_POWERUP_SP; 
    // 0b0011_0100 or 0x34
    // cpu->p = (uint8_t)cpu_flag_ignored | (uint8_t)cpu_flag_break | (uint8_t)cpu_flag_interrupt_disable; 
    set_flag_carry(cpu, 0); 
    set_flag_zero(cpu, 0); 
    set_flag_IRQ(cpu, 1); 
    set_flag_decimal(cpu, 0); 
    set_flag_overflow(cpu, 0); 
    set_flag_negative(cpu, 0);
    cpu->pc = 0; // ? 
}
/// https://www.nesdev.org/wiki/CPU_power_up_state 
void cpu_reset(struct nescpu *cpu) {
    cpu->sp -= 3; 
    set_flag_IRQ(cpu, 1); 
    cpu->pc = cpu_read16(cpu, CPU_RESET_VECTOR); 
}


uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr) {
    panic("TODO"); 
    return 0;
}
void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value) {
    panic("TODO"); 
}

uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr) {
    return (
        ((uint16_t)cpu_read8(cpu, addr)) | 
        (((uint16_t)cpu_read8(cpu, addr+1)) << 8)
    );
}
void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value) {
    cpu_write8(cpu, addr,   (uint8_t)(value & 0xFF));
    cpu_write8(cpu, addr+1, (uint8_t)(value >> 8)); 
}


