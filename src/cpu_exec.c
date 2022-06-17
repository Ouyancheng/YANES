#include "cpu.h"
#include "cpu_opcodes.h"

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
        case MODE_IMP: 
            break;
        case MODE_ABS: 
            addr = cpu_read16(cpu, ptr); 
            break;
        case MODE_ZPG: 
            addr = (uint16_t)cpu_read8(cpu, ptr); 
            break;
        case MODE_ZPX: 
            addr = (uint16_t)((cpu_read8(cpu, ptr) + cpu->x) & 0xFF);
            break;
        case MODE_ZPY: 
            addr = (uint16_t)((cpu_read8(cpu, ptr) + cpu->y) & 0xFF);
            break;
        case MODE_ABX: {
            uint16_t base = cpu_read16(cpu, ptr);
            addr = base + (uint16_t)cpu->x; 
            page_crossed = check_page_crossed(base, addr); 
            break;
        }
        case MODE_ABY: {
            uint16_t base = cpu_read16(cpu, ptr);
            addr = base + (uint16_t)cpu->y; 
            page_crossed = check_page_crossed(base, addr); 
            break;
        }
        case MODE_REL: {
            int8_t offset = cpu_read8(cpu, ptr); 
            addr = cpu->pc + 1 + (uint16_t)((int16_t)offset); // sign extend
            page_crossed = check_page_crossed(cpu->pc + 1, addr);
            break; 
        }
        case MODE_IMM: 
            addr = ptr;
            break; 
        case MODE_IND: 
            addr = cpu_read16(cpu, cpu_read16(cpu, ptr)); 
            break;
        case MODE_INX: {
            uint8_t base = cpu_read8(cpu, ptr); 
            base += cpu->x;
            addr = cpu_read16(cpu, (uint16_t)base); 
            break;
        }
        case MODE_INY: {
            uint16_t base = cpu_read16(cpu, (uint16_t)cpu_read8(cpu, ptr)); 
            addr = base + cpu->y; 
            page_crossed = check_page_crossed(base, addr); 
            break;
        }
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

internal_function int single_step(struct nescpu *cpu) {
    uint8_t opcode = cpu_read8(cpu, cpu->pc); 
    struct cpu_opcode opcode_entry = opcode_table[opcode];  
    cpu->pc += 1; // increment the PC by 1 so that it's pointing at the address byte 

    // TODO: switch on instruction 

    // TODO: add pc with the instruction length - 1 

    // TODO: handle cycles_taken 
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


