#include "cpu.h"
#include "cpu_addressmode.h"
#include "cpu_instruction.h"


#define internal_function static inline

internal_function void set_flag(struct nescpu *cpu, uint8_t flag_mask, bool val) {
    cpu->p = (val) ? (cpu->p | flag_mask) : (cpu->p & (~flag_mask)); 
}

internal_function bool flag_is_set(struct nescpu *cpu, uint8_t flag_mask) { 
    return cpu->p & flag_mask; 
}

internal_function void update_zero_negative_flags(struct nescpu *cpu, uint8_t according_to_value) {
    set_flag(cpu, CPU_FLAG_Z, according_to_value == 0); 
    set_flag(cpu, CPU_FLAG_N, (according_to_value >> 7)); 
}

internal_function bool check_page_crossed(uint16_t base, uint16_t target) {
    return (base & 0xFF00) != (target & 0xFF00); 
}

uint16_t cpu_compute_address(struct nescpu *cpu, uint8_t addrmode, uint16_t ptr, bool *is_page_crossed) {
    bool page_crossed = false; 
    uint16_t addr = 0; 
    switch (addrmode) {
        case MODE_IMP: ADDRMODE_GETADDR(MODE_IMP)(addr, cpu, ptr, page_crossed); break;
        case MODE_ABS: ADDRMODE_GETADDR(MODE_ABS)(addr, cpu, ptr, page_crossed); break;
        case MODE_ZPG: ADDRMODE_GETADDR(MODE_ZPG)(addr, cpu, ptr, page_crossed); break;
        case MODE_ZPX: ADDRMODE_GETADDR(MODE_ZPX)(addr, cpu, ptr, page_crossed); break;
        case MODE_ZPY: ADDRMODE_GETADDR(MODE_ZPY)(addr, cpu, ptr, page_crossed); break;
        case MODE_ABX: ADDRMODE_GETADDR(MODE_ABX)(addr, cpu, ptr, page_crossed); break;
        case MODE_ABY: ADDRMODE_GETADDR(MODE_ABY)(addr, cpu, ptr, page_crossed); break;
        case MODE_REL: ADDRMODE_GETADDR(MODE_REL)(addr, cpu, ptr, page_crossed); break;
        case MODE_IMM: ADDRMODE_GETADDR(MODE_IMM)(addr, cpu, ptr, page_crossed); break;
        case MODE_IND: ADDRMODE_GETADDR(MODE_IND)(addr, cpu, ptr, page_crossed); break;
        case MODE_INX: ADDRMODE_GETADDR(MODE_INX)(addr, cpu, ptr, page_crossed); break;
        case MODE_INY: ADDRMODE_GETADDR(MODE_INY)(addr, cpu, ptr, page_crossed); break;
        default:
            panic("invalid addressmode %u\n", (unsigned)addrmode); 
            break;
    }
    (*is_page_crossed) = page_crossed; 
    return addr; 
}

internal_function void add_to_A(struct nescpu *cpu, uint8_t value) {
    uint32_t sum = (uint32_t)cpu->a + (uint32_t)value; 
    sum += flag_is_set(cpu, CPU_FLAG_C); 
    set_flag(cpu, CPU_FLAG_C, sum > 0xFF); 
    uint8_t result = (uint8_t)sum; 
    set_flag(cpu, CPU_FLAG_V, ((value ^ result) & (result ^ cpu->a) & 0x80) != 0);
    cpu->a = result;
    update_zero_negative_flags(cpu, result); 
}

internal_function void subtract_from_A(struct nescpu *cpu, uint8_t value) {
    add_to_A(cpu, (uint8_t)((-(int8_t)value) - 1)); 
}

internal_function unsigned branch(struct nescpu *cpu, bool cond, uint16_t taken_addr, bool page_crossed) {
    unsigned total_cycles = 0;
    if (cond) {
        total_cycles += 1;
        cpu->pc = taken_addr;
        total_cycles += page_crossed;
    }
    return total_cycles;
}
internal_function unsigned compare(struct nescpu *cpu, uint8_t compare_with, uint16_t addr, bool page_crossed) {
    uint8_t data = cpu_read8(cpu, addr); 
    set_flag(cpu, CPU_FLAG_C, data <= compare_with); 
    update_zero_negative_flags(cpu, compare_with - data); 
    return page_crossed;
}

internal_function uint8_t pop_u8(struct nescpu *cpu) {
    cpu->sp += 1; 
    return cpu_read8(cpu, CPU_STACK_OFFSET + (uint16_t)(cpu->sp)); 
}
internal_function void push_u8(struct nescpu *cpu, uint8_t val) {
    cpu_write8(cpu, CPU_STACK_OFFSET + (uint16_t)(cpu->sp), val); 
    cpu->sp -= 1; 
}
internal_function void push_u16(struct nescpu *cpu, uint16_t val) {
    push_u8(cpu, (uint8_t)(val >> 8));
    push_u8(cpu, (uint8_t)(val & 0xff)); 
}
internal_function uint16_t pop_u16(struct nescpu *cpu) {
    uint16_t lo = (uint16_t)pop_u8(cpu); 
    uint16_t hi = (uint16_t)pop_u8(cpu); 
    return ((hi << 8) | lo);
}

typedef struct nescpu cpu_t;
#define EXEC(INSTR) internal_function unsigned exec_##INSTR(cpu_t *cpu, uint16_t addr, bool page_crossed)
EXEC(ADC) {
    add_to_A(cpu, cpu_read8(cpu, addr)); 
    return page_crossed;
}
EXEC(AND) {
    cpu->a &= cpu_read8(cpu, addr);
    update_zero_negative_flags(cpu, cpu->a); 
    return page_crossed;
}
EXEC(ASL) {
    uint8_t value = cpu_read8(cpu, addr); 
    set_flag(cpu, CPU_FLAG_C, value >> 7);
    value <<= 1; 
    cpu_write8(cpu, addr, value);
    update_zero_negative_flags(cpu, value); 
    return 0;
}
EXEC(BCC) {
    return branch(cpu, !flag_is_set(cpu, CPU_FLAG_C), addr, page_crossed);
}
EXEC(BCS) {
    return branch(cpu, flag_is_set(cpu, CPU_FLAG_C), addr, page_crossed);
}
EXEC(BEQ) {
    return branch(cpu, flag_is_set(cpu, CPU_FLAG_Z), addr, page_crossed);
}
EXEC(BIT) {
    uint8_t data = cpu_read8(cpu, addr); 
    uint8_t and = cpu->a & data; 
    set_flag(cpu, CPU_FLAG_Z, !and);
    set_flag(cpu, CPU_FLAG_N, data & UINT8_C(0b10000000));
    set_flag(cpu, CPU_FLAG_V, data & UINT8_C(0b01000000));
    return 0;
}
EXEC(BMI) {
    return branch(cpu, flag_is_set(cpu, CPU_FLAG_N), addr, page_crossed);
}
EXEC(BNE) {
    return branch(cpu, !flag_is_set(cpu, CPU_FLAG_Z), addr, page_crossed);
}
EXEC(BPL) {
    return branch(cpu, !flag_is_set(cpu, CPU_FLAG_N), addr, page_crossed); 
}
EXEC(BRK) {
    panic("BRK occurred, but not handled\n");
    return 0; 
}
EXEC(BVC) {
    return branch(cpu, !flag_is_set(cpu, CPU_FLAG_V), addr, page_crossed);
}
EXEC(BVS) {
    return branch(cpu, flag_is_set(cpu, CPU_FLAG_V), addr, page_crossed); 
}
EXEC(CLC) {
    set_flag(cpu, CPU_FLAG_C, false); 
    return 0;
}
EXEC(CLD) {
    set_flag(cpu, CPU_FLAG_D, false);
    return 0;
}
EXEC(CLI) {
    set_flag(cpu, CPU_FLAG_I, false);
    return 0;
}
EXEC(CLV) {
    set_flag(cpu, CPU_FLAG_V, false);
    return 0;
}
EXEC(CMP) {
    return compare(cpu, cpu->a, addr, page_crossed); 
}
EXEC(CPX) {
    return compare(cpu, cpu->x, addr, page_crossed); 
}
EXEC(CPY) {
    return compare(cpu, cpu->y, addr, page_crossed); 
}
EXEC(DEC) {
    uint8_t data = cpu_read8(cpu, addr);
    data -= 1;
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    return 0;
}
EXEC(DEX) {
    cpu->x -= 1;
    update_zero_negative_flags(cpu, cpu->x);
    return 0;
}
EXEC(DEY) {
    cpu->y -= 1;
    update_zero_negative_flags(cpu, cpu->y);
    return 0;
}
EXEC(EOR) {
    uint8_t data = cpu_read8(cpu, addr);
    cpu->a ^= data;
    update_zero_negative_flags(cpu, cpu->a); 
    return page_crossed;
}
EXEC(INC) {
    uint8_t data = cpu_read8(cpu, addr);
    data += 1;
    cpu_write8(cpu, addr, data);
    update_zero_negative_flags(cpu, data);
    return 0;
}
EXEC(INX) {
    cpu->x += 1;
    update_zero_negative_flags(cpu, cpu->x);
    return 0;
}
EXEC(INY) {
    cpu->y += 1;
    update_zero_negative_flags(cpu, cpu->y);
    return 0;
}
EXEC(JMP) {
    cpu->pc = addr;
    return 0;
}
EXEC(JSR) {
    push_u16(cpu, cpu->pc + 2 - 1);
    cpu->pc = addr;
    return 0; 
}
EXEC(LDA) {
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->a = data; 
    update_zero_negative_flags(cpu, data); 
    return page_crossed;
}
EXEC(LDX) {
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->x = data; 
    update_zero_negative_flags(cpu, data); 
    return page_crossed;
}
EXEC(LDY) {
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->y = data; 
    update_zero_negative_flags(cpu, data); 
    return page_crossed;
}
EXEC(LSR) {
    uint8_t data = cpu_read8(cpu, addr); 
    set_flag(cpu, CPU_FLAG_C, data & 1); 
    data >>= 1; 
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    return 0; 
}
EXEC(NOP) {
    return 0; 
}
EXEC(ORA) {
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->a |= data; 
    update_zero_negative_flags(cpu, cpu->a); 
    return page_crossed; 
}
EXEC(PHA) {
    push_u8(cpu, cpu->a); 
    return 0;
}
EXEC(PHP) {
    uint8_t flags = cpu->p; 
    flags |= CPU_FLAG_B; 
    flags |= CPU_FLAG__; 
    push_u8(cpu, flags);
    return 0;
}
EXEC(PLA) {
    cpu->a = pop_u8(cpu); 
    update_zero_negative_flags(cpu, cpu->a);
    return 0;
}
EXEC(PLP) {
    uint8_t flags = pop_u8(cpu);
    flags &= ~CPU_FLAG_B;
    flags |= CPU_FLAG__;
    cpu->p = flags; 
    return 0;
}
EXEC(ROL) {
    uint8_t data = cpu_read8(cpu, addr); 
    bool carry_bit = flag_is_set(cpu, CPU_FLAG_C);
    set_flag(cpu, CPU_FLAG_C, data >> 7);
    data <<= 1; 
    data |= carry_bit;
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    return 0; 
}
EXEC(ROR) {
    uint8_t data = cpu_read8(cpu, addr); 
    uint8_t carry_bit = flag_is_set(cpu, CPU_FLAG_C);
    set_flag(cpu, CPU_FLAG_C, data & 1);
    data >>= 1; 
    if (carry_bit) {
        data |= UINT8_C(0b10000000); 
    }
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    return 0; 
}
EXEC(RTI) {
    cpu->p = pop_u8(cpu); 
    cpu->p &= ~CPU_FLAG_B; 
    cpu->p |= CPU_FLAG__; 
    cpu->pc = pop_u16(cpu); 
    return 0; 
}
EXEC(RTS) {
    cpu->pc = pop_u16(cpu) + 1;
    return 0;
}
EXEC(SBC) {
    uint8_t data = cpu_read8(cpu, addr); 
    subtract_from_A(cpu, data);
    return page_crossed;
}
EXEC(SEC) {
    set_flag(cpu, CPU_FLAG_C, true);
    return 0;
}
EXEC(SED) {
    set_flag(cpu, CPU_FLAG_D, true);
    return 0;
}
EXEC(SEI) {
    set_flag(cpu, CPU_FLAG_I, true); 
    return 0;
}
EXEC(STA) {
    cpu_write8(cpu, addr, cpu->a); 
    return 0;
}
EXEC(STX) {
    cpu_write8(cpu, addr, cpu->x); 
    return 0;
}
EXEC(STY) {
    cpu_write8(cpu, addr, cpu->y); 
    return 0;
}
EXEC(TAX) {
    cpu->x = cpu->a; 
    update_zero_negative_flags(cpu, cpu->x); 
    return 0;
}
EXEC(TAY) {
    cpu->y = cpu->a; 
    update_zero_negative_flags(cpu, cpu->y); 
    return 0;
}
EXEC(TSX) {
    cpu->x = cpu->sp;
    update_zero_negative_flags(cpu, cpu->x);
    return 0;
}
EXEC(TXA) {
    cpu->a = cpu->x; 
    update_zero_negative_flags(cpu, cpu->a); 
    return 0;
}
EXEC(TXS) {
    cpu->sp = cpu->x;
    return 0;
}
EXEC(TYA) {
    cpu->a = cpu->y; 
    update_zero_negative_flags(cpu, cpu->a); 
    return 0;
}
EXEC(ASL_A) {
    uint8_t value = cpu->a; 
    set_flag(cpu, CPU_FLAG_C, value >> 7);
    value <<= 1; 
    cpu->a = value;
    update_zero_negative_flags(cpu, value); 
    return 0;
}
EXEC(LSR_A) {
    uint8_t data = cpu->a; 
    set_flag(cpu, CPU_FLAG_C, data & 1); 
    data >>= 1; 
    cpu->a = data;
    update_zero_negative_flags(cpu, data); 
    return 0; 
}
EXEC(ROL_A) {
    uint8_t data = cpu->a; 
    bool carry_bit = flag_is_set(cpu, CPU_FLAG_C);
    set_flag(cpu, CPU_FLAG_C, data >> 7);
    data <<= 1; 
    data |= carry_bit;
    cpu->a = data;
    update_zero_negative_flags(cpu, data); 
    return 0; 
}
EXEC(ROR_A) {
    uint8_t data = cpu->a; 
    uint8_t carry_bit = flag_is_set(cpu, CPU_FLAG_C);
    set_flag(cpu, CPU_FLAG_C, data & 1);
    data >>= 1; 
    if (carry_bit) {
        data |= UINT8_C(0b10000000); 
    }
    cpu->a = data; 
    update_zero_negative_flags(cpu, data); 
    return 0; 
}
EXEC(ALR_) { // AND oper + LSR: A AND oper, 0 -> [76543210] -> C
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->a &= data; 
    update_zero_negative_flags(cpu, cpu->a); 
    exec_LSR_A(cpu, addr, page_crossed); 
    return 0;
}
EXEC(ANC_) { // 0B: AND oper + set C as ASL: A AND oper, bit(7) -> C
// 2B: AND oper + set C as ROL: effectively the same as instr. 0B: A AND oper, bit(7) -> C
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->a &= data;
    update_zero_negative_flags(cpu, cpu->a);
    set_flag(cpu, CPU_FLAG_C, flag_is_set(cpu, CPU_FLAG_N)); 
    return 0;
}
EXEC(ARR_) { // AND oper + ROR: A AND oper, C -> [76543210] -> C 
    /// TODO: validate this
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->a &= data;
    update_zero_negative_flags(cpu, cpu->a); 
    exec_ROR_A(cpu, addr, page_crossed); 
    uint8_t result = cpu->a; 
    uint8_t b5 = (result >> 5) & 1; 
    uint8_t b6 = (result >> 6) & 1; 
    set_flag(cpu, CPU_FLAG_C, b6); // The carry is not set, but bit 7 (sign) is exchanged with the carry
    set_flag(cpu, CPU_FLAG_V, b5 != b6); // V-flag is set according to (A AND oper) + oper
    // registers? 
    update_zero_negative_flags(cpu, result); 
    return 0;
}
EXEC(AXS_) { // SBX (AXS, SAX): CMP and DEX at once, sets flags like CMP: (A AND X) - oper -> X
    uint8_t data = cpu_read8(cpu, addr); 
    uint8_t xa = cpu->x & cpu->a; 
    uint8_t result = xa - data; 
    set_flag(cpu, CPU_FLAG_C, data <= xa); 
    update_zero_negative_flags(cpu, result); 
    cpu->x = result; 
    return 0; 
}
EXEC(LAX_) { // LDA oper + LDX oper: M -> A -> X
/* NOTE: 0xAB: LXA (LAX immediate) 
    Store * AND oper in A and X
    Highly unstable, involves a 'magic' constant, see ANE(XAA)
    (A OR CONST) AND oper -> A -> X
*/
    uint8_t data = cpu_read8(cpu, addr); 
    cpu->a = data;
    cpu->x = data;
    update_zero_negative_flags(cpu, data);
    return 0;
}
EXEC(SAX_) { // A and X are put on the bus at the same time (resulting effectively in an AND operation) and stored in M
// A AND X -> M
    cpu_write8(cpu, addr, cpu->a & cpu->x); 
    return 0; 
}
EXEC(XAA_) { // * AND X + AND oper: (A OR CONST) AND X AND oper -> A
    cpu->a &= cpu->x; 
    update_zero_negative_flags(cpu, cpu->a); 
    cpu->a &= cpu_read8(cpu, addr);
    update_zero_negative_flags(cpu, cpu->a);
    return 0;
}
EXEC(AHX_) { // Stores A AND X AND (high-byte of addr. + 1) at addr.
// A AND X AND (H+1) -> M
/*
unstable: sometimes 'AND (H+1)' is dropped, 
page boundary crossings may not work (with the high-byte of the value used as the high-byte of the address)
*/
    /// TODO: fix this
    cpu_write8(cpu, addr, cpu->a & cpu->x & ((uint8_t)(addr >> 8)) + 1); 
    return page_crossed;
}
EXEC(TAS_) { // Puts A AND X in SP and stores A AND X AND (high-byte of addr. + 1) at addr.: A AND X -> SP, A AND X AND (H+1) -> M
    uint8_t data = cpu->a & cpu->x; 
    cpu->sp = data; 
    uint16_t mem_address = cpu_read16(cpu, cpu->pc) + (uint16_t)cpu->y; 
    data = ((uint8_t)(mem_address >> 8) + 1) & cpu->sp;
    cpu_write8(cpu, mem_address, data);
    return 0;
}
EXEC(LAS_) { // LDA/TSX oper: M AND SP -> A, X, SP
    uint8_t data = cpu_read8(cpu, addr); 
    data &= cpu->sp;
    cpu->a = data; 
    cpu->x = data; 
    cpu->sp = data; 
    update_zero_negative_flags(cpu, data); 
    return 0;
}
EXEC(DCP_) { // DEC oper + CMP oper: M - 1 -> M, A - M
    uint8_t data = cpu_read8(cpu, addr); 
    data -= 1; 
    cpu_write8(cpu, addr, data); 
    set_flag(cpu, CPU_FLAG_C, data <= cpu->a); 
    // if (data <= cpu->a) {
    //     set_flag(cpu, CPU_FLAG_C, true); 
    // }
    update_zero_negative_flags(cpu, cpu->a - data); 
    return 0; 
}
EXEC(ISC_) { // INC oper + SBC oper: M + 1 -> M, A - M - (neg C) -> A
    uint8_t data = cpu_read8(cpu, addr);
    data += 1;
    cpu_write8(cpu, addr, data);
    update_zero_negative_flags(cpu, data);
    subtract_from_A(cpu, data); 
    return 0; 
}
EXEC(RLA_) { // ROL oper + AND oper: M = C <- [76543210] <- C, A AND M -> A
    uint8_t data = cpu_read8(cpu, addr); 
    bool carry_bit = flag_is_set(cpu, CPU_FLAG_C);
    set_flag(cpu, CPU_FLAG_C, data >> 7);
    data <<= 1; 
    data |= carry_bit;
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    cpu->a &= data;
    update_zero_negative_flags(cpu, cpu->a); 
    return 0; 
}
EXEC(RRA_) { // ROR oper + ADC oper: M = C -> [76543210] -> C, A + M + C -> A, C
    uint8_t data = cpu_read8(cpu, addr); 
    uint8_t carry_bit = flag_is_set(cpu, CPU_FLAG_C);
    set_flag(cpu, CPU_FLAG_C, data & 1);
    data >>= 1; 
    if (carry_bit) {
        data |= UINT8_C(0b10000000); 
    }
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    add_to_A(cpu, data); 
    return 0; 
}
EXEC(SLO_) { // ASL oper + ORA oper: M = C <- [76543210] <- 0, A OR M -> A
    uint8_t data = cpu_read8(cpu, addr); 
    set_flag(cpu, CPU_FLAG_C, data >> 7);
    data <<= 1; 
    cpu_write8(cpu, addr, data);
    update_zero_negative_flags(cpu, data); 
    cpu->a |= data; 
    update_zero_negative_flags(cpu, cpu->a); 
    return 0;
}
EXEC(SRE_) { // LSR oper + EOR oper: M = 0 -> [76543210] -> C, A EOR M -> A
    uint8_t data = cpu_read8(cpu, addr); 
    set_flag(cpu, CPU_FLAG_C, data & 1); 
    data >>= 1; 
    cpu_write8(cpu, addr, data); 
    update_zero_negative_flags(cpu, data); 
    cpu->a ^= data;
    update_zero_negative_flags(cpu, cpu->a);
    return 0; 
}
EXEC(SHY_) { // Stores Y AND (high-byte of addr. + 1) at addr.: Y AND (H+1) -> M
    uint16_t mem_address = cpu_read16(cpu, cpu->pc) + (uint16_t)cpu->x; 
    uint8_t data = cpu->y & ((uint8_t)(mem_address >> 8) + 1); 
    cpu_write8(cpu, mem_address, data); 
    return page_crossed;
}
EXEC(SHX_) { // Stores X AND (high-byte of addr. + 1) at addr.: X AND (H+1) -> M
/*
unstable: sometimes 'AND (H+1)' is dropped, 
page boundary crossings may not work (with the high-byte of the value used as the high-byte of the address)
*/
    uint16_t mem_address = cpu_read16(cpu, cpu->pc) + (uint16_t)cpu->y; 
    // if cross page boundry {
    //     mem_address &= ((uint16_t)cpu.x) << 8;
    // }
    uint8_t data = cpu->x & ((uint8_t)(mem_address >> 8) + 1); 
    cpu_write8(cpu, mem_address, data); 
    return page_crossed;
}
EXEC(SBC_) { // SBC oper + NOP: effectively same as normal SBC immediate, instr. E9.: A - M - C -> A
    return exec_SBC(cpu, addr, page_crossed); 
}
EXEC(NOP_) {
    /// TODO: some nops has read actions (Maybe?)
    return page_crossed;
}
EXEC(STP_) {
    panic("CPU halted!\n"); 
    return 0; 
}
#define OPCODE_CASEBRANCH(OPCODE_HEX) \
case OPCODE_HEX: { \
    OP_GETADDR(OPCODE_HEX)(addr, cpu, cpu->pc, page_crossed); \
    additional_cycles = EXECINSTR(OP_GETINSTR(OPCODE_HEX))(cpu, addr, page_crossed);\
    instr_length = OP_GETLENGTH(OPCODE_HEX);\
    base_cycle = OP_GETCYC(OPCODE_HEX);\
} break;






internal_function int single_step(struct nescpu *cpu) {
    uint8_t opcode = cpu_read8(cpu, cpu->pc); 
    // struct cpu_opcode opcode_entry = opcode_table[opcode];  
    cpu->pc += 1; // increment the PC by 1 so that it's pointing at the address byte 
    uint16_t prev_pc = cpu->pc;
    uint16_t addr;
    bool page_crossed = false;
    unsigned additional_cycles;
    unsigned instr_length;
    unsigned base_cycle;
    // switch on instruction 
    switch (opcode) {
        OPCODE_CASES(OPCODE_CASEBRANCH)
        default: 
            panic("invalid opcode %02x\n", opcode); 
            break;
    }
    // add pc with the instruction length - 1 if it's not a branch 
    if (cpu->pc == prev_pc) {
        cpu->pc += (instr_length - 1);
    }
    // TODO: handle cycles_taken 
    base_cycle += additional_cycles;

    return 0;
}

int cpu_single_step(struct nescpu *cpu) { return single_step(cpu); }
void cpu_run(struct nescpu *cpu, void(*callback)(struct nescpu *c)) {
    int single_step_outcome; 
    while (1) {
        if (callback) {
            callback(cpu); 
        }
        single_step_outcome = single_step(cpu); 
        if (single_step_outcome < 0) {
            break; 
        }
    }
}


