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
internal_function void update_zero_negative_flags(struct nescpu *cpu, uint8_t according_to_value) {
    set_flag_zero(cpu, according_to_value == 0); 
    set_flag_negative(cpu, (according_to_value >> 7)); 
}
internal_function bool check_page_crossed(uint16_t base, uint16_t target) {
    return (base & UINT16_C(0xFF00)) != (target & UINT16_C(0xFF00)); 
}
internal_function uint16_t compute_address(struct nescpu *cpu, enum addressmode addrmode, uint16_t ptr, bool *page_crossed) {
    bool address_page_crossed = false; 
    uint16_t addr = 0; 
    switch (addrmode) {
        case addressmode_accumulator:
        case addressmode_implicit: 
            break;
        case addressmode_absolute: 
            addr = cpu_read16(cpu, ptr); 
            break;
        case addressmode_zeropage: 
            addr = (uint16_t)cpu_read8(cpu, ptr); 
            break;
        case addressmode_zeropage_x: 
            addr = (uint16_t)((cpu_read8(cpu, ptr) + cpu->x) & 0xFF);
            break;
        case addressmode_zeropage_y: 
            addr = (uint16_t)((cpu_read8(cpu, ptr) + cpu->y) & 0xFF);
            break;
        case addressmode_absolute_x: {
            uint16_t base = cpu_read16(cpu, ptr);
            addr = base + (uint16_t)cpu->x; 
            address_page_crossed = check_page_crossed(base, addr); 
            break;
        }
        case addressmode_absolute_y: {
            uint16_t base = cpu_read16(cpu, ptr);
            addr = base + (uint16_t)cpu->y; 
            address_page_crossed = check_page_crossed(base, addr); 
            break;
        }
        case addressmode_relative: {
            int8_t offset = cpu_read8(cpu, ptr); 
            addr = cpu->pc + 1 + (uint16_t)((int16_t)offset); // sign extend
            address_page_crossed = check_page_crossed(cpu->pc + 1, addr);
            break; 
        }
        case addressmode_immediate: 
            addr = ptr;
            break; 
        case addressmode_indirect: 
            addr = cpu_read16(cpu, cpu_read16(cpu, ptr)); 
            break;
        case addressmode_indirect_x: {
            uint8_t base = cpu_read8(cpu, ptr); 
            base += cpu->x;
            addr = cpu_read16(cpu, (uint16_t)base); 
            break;
        }
        case addressmode_indirect_y: {
            uint16_t base = cpu_read16(cpu, (uint16_t)cpu_read8(cpu, ptr)); 
            addr = base + cpu->y; 
            address_page_crossed = check_page_crossed(base, addr); 
            break;
        }
        default:
            panic("invalid addressmode %u\n", (unsigned)addrmode); 
            break;
    }
    (*page_crossed) = address_page_crossed; 
    return addr; 
}
internal_function void add_to_A(struct nescpu *cpu, uint8_t value) {
    uint16_t sum = (uint16_t)cpu->a + (uint16_t)value; 
    sum += get_flag_carry(cpu); 
    set_flag_carry(cpu, sum > UINT16_C(0xFF)); 
    uint8_t result = (uint8_t)sum; 
    set_flag_overflow(cpu, ((value ^ result) & (result ^ cpu->a) & 0x80) != 0);
    cpu->a = result;
    update_zero_negative_flags(cpu, result); 
}
internal_function void subtract_from_A(struct nescpu *cpu, uint8_t value) {
    add_to_A(cpu, (uint8_t)((-(int8_t)value) - 1)); 
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
internal_function unsigned execute_type1_ALU_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    assert(0 <= aaa && aaa < 8); 
    assert(0 <= bbb && bbb < 8); 
    bool pagecrossed = false;
    const struct bbb_table_entry *bbb_entry = &(type1_bbb_lookup_table[bbb]);
    enum addressmode mode = bbb_entry->mode;
    unsigned base_cycles = bbb_entry->base_cycle;  
    uint16_t address = compute_address(cpu, mode, cpu->pc, &pagecrossed);
    switch (aaa) {
        case 0b000: // ORA
            cpu->a |= cpu_read8(cpu, address); 
            update_zero_negative_flags(cpu, cpu->a); 
            break;
        case 0b001: // AND
            cpu->a &= cpu_read8(cpu, address);
            update_zero_negative_flags(cpu, cpu->a); 
            break;
        case 0b010: // EOR 
            cpu->a ^= cpu_read8(cpu, address); 
            update_zero_negative_flags(cpu, cpu->a); 
            break;
        case 0b011: // ADC 
            add_to_A(cpu, cpu_read8(cpu, address)); 
            break;
        case 0b100: // STA 
            if (mode != addressmode_immediate) {
                cpu_write8(cpu, address, cpu->a); 
                pagecrossed = true;
            }
            break;
        case 0b101: // LDA
            cpu->a = cpu_read8(cpu, address); 
            update_zero_negative_flags(cpu, cpu->a); 
            break;
        case 0b110: { // CMP
            uint8_t data = cpu_read8(cpu, address); 
            set_flag_carry(cpu, data <= cpu->a); 
            update_zero_negative_flags(cpu, cpu->a - data); 
            break;
        }
        case 0b111: // SBC 
            subtract_from_A(cpu, cpu_read8(cpu, address)); 
            break; 
        default: 
            panic("invalid aaa %u\n", aaa); 
            break; 
    }
    cpu->pc += (bbb_entry->length - 1);
    return base_cycles + pagecrossed; 
}
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
internal_function unsigned execute_type2_RMW_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    assert(0 <= aaa && aaa < 8); 
    assert(0 <= bbb && bbb < 8); 
    bool pagecrossed = false;
    const struct bbb_table_entry *bbb_entry = &(type2_bbb_lookup_table[bbb]); 
    if (bbb == 0b100) {
        // panic("opcode aaa=%x bbb=%x cc=10 falls into halting condition\n", aaa, bbb); 
        cpu->pc += (bbb_entry->length - 1); 
        return bbb_entry->base_cycle; 
    }
    enum addressmode mode = bbb_entry->mode; 
    uint16_t address = compute_address(cpu, bbb_entry->mode, cpu->pc, &pagecrossed); 
    switch (aaa) {
        case 0b000: { // ASL
            uint8_t value = (mode == addressmode_accumulator) ? cpu->a : cpu_read8(cpu, address); 
            set_flag_carry(cpu, value >> 7); 
            value <<= 1;
            if (mode == addressmode_accumulator) {
                cpu->a = value; 
            } else {
                cpu_write8(cpu, address, value); 
            }
            update_zero_negative_flags(cpu, value); 
            break;
        }
        case 0b001: { // ROL
            uint8_t value = (mode == addressmode_accumulator) ? cpu->a : cpu_read8(cpu, address); 
            uint8_t prev_carry = get_flag_carry(cpu); 
            set_flag_carry(cpu, value >> 7); 
            value = ((value << 1) | prev_carry);
            if (mode == addressmode_accumulator) {
                cpu->a = value; 
            } else {
                cpu_write8(cpu, address, value); 
            }
            update_zero_negative_flags(cpu, value); 
            break;
        }
        case 0b010: { // LSR
            uint8_t value = (mode == addressmode_accumulator) ? cpu->a : cpu_read8(cpu, address); 
            set_flag_carry(cpu, value & 1); 
            value >>= 1;
            if (mode == addressmode_accumulator) {
                cpu->a = value; 
            } else {
                cpu_write8(cpu, address, value); 
            }
            update_zero_negative_flags(cpu, value); 
            break;
        }
        case 0b011: { // ROR
            uint8_t value = (mode == addressmode_accumulator) ? cpu->a : cpu_read8(cpu, address); 
            uint8_t prev_carry = get_flag_carry(cpu); 
            set_flag_carry(cpu, value & 1); 
            value = ((value >> 1) | (prev_carry << 7));
            if (mode == addressmode_accumulator) {
                cpu->a = value; 
            } else {
                cpu_write8(cpu, address, value); 
            }
            update_zero_negative_flags(cpu, value); 
            break;
        }
        case 0b100: // STX

            break;
        case 0b101: // LDX
        case 0b110: // DEC
        case 0b111: // INC
        default: 
            panic("invalid aaa %u\n", aaa); 
            break;
    }
    cpu->pc += (bbb_entry->length - 1); 
    return bbb_entry->base_cycle; 
}
internal_function unsigned execute_type0_control_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    assert(0 <= aaa && aaa < 8); 
    assert(0 <= bbb && bbb < 8); 
    panic("TODO\n"); 
    return 0;
}
internal_function unsigned execute_type3_unofficial_instruction(struct nescpu *cpu, const struct cpu_opcode *opcode_entry, unsigned aaa, unsigned bbb) {
    assert(0 <= aaa && aaa < 8); 
    assert(0 <= bbb && bbb < 8); 
    panic("TODO: opcode aaa=%x bbb=%x cc=11 is an unofficial opcode\n", aaa, bbb); 
    return 0; 
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
/// https://www.pagetable.com/?p=410 
void cpu_reset(struct nescpu *cpu) {
    cpu_init(cpu); // technically speaking this is not quite right, we don't need this
    cpu->sp = CPU_POWERUP_SP; // it seems like it's 0x00 - 3 = 0xFD, not 0xFD - 3... 
    set_flag_IRQ(cpu, 1); 
    cpu->pc = cpu_read16(cpu, CPU_RESET_VECTOR); 
}


extern inline uint8_t cpu_read8(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write8(struct nescpu *cpu, uint16_t addr, uint8_t value); 

extern inline uint16_t cpu_read16(struct nescpu *cpu, uint16_t addr); 
extern inline void cpu_write16(struct nescpu *cpu, uint16_t addr, uint16_t value); 

