#include "cpu.h"
#include "cpu_instruction.h"
#include "cpu_addressmode.h"
#include "bus.h"
#include "rom.h"
#include "mappers/mapper0.h"
void test_snake(void);

void print_current_instr(struct nescpu *cpu) {
    printf("%04X  %s\n", cpu->pc, opcode_nametable[bus_read8(cpu->bus, cpu->pc)]);
}
void dump_cpu(struct nescpu *cpu);
int main(int argc, const char *const argv[]) {
#if 0
    print_cpu_opcode_nametable(); 
    #define CASE_N(OP) case OP: printf("#define OP_INSTR_0x%02X (%s, %d)\n", OP, INSTR_TO_STR(OP_GETINSTR(OP)), OP_GETCYC(OP)); break;
    for (unsigned k = 0; k <= 3; ++k) {
        for (unsigned j = 0x00; j <= 0x1F; j += 4) {
            for (unsigned i = 0x00; i <= 0xE0; i += 0x20) {
                switch (i+j+k) {
                    OPCODE_CASES(CASE_N)
                }
            }
            printf("\n");
        }
        printf("///////////////////////////////////////////////////////////\n");
    }
#endif 
    struct nescpu cpu;
    struct nesbus bus;
    // bool dummy;
    // printf("test compute addr %d\n", cpu_compute_address(&cpu, MODE_IMP, 0, &dummy));
    
    size_t datasize = 0;
    uint8_t *rom_data = load_file("../testroms/nestest.nes", &datasize);
    bus_init(&bus);
    if (load_rom_from_ines_data(&bus, rom_data, datasize) < 0) {
        panic("Load failed\n"); 
    }
    // printf("bus.PRG_reader = %p, NROM_read_PRG = %p\n", bus.PRG_reader, NROM_read_PRG);
    // printf("%u\n", (unsigned)NROM_read_PRG(bus.rom, 0x8000));
    assert(bus.PRG_reader == NROM_read_PRG);
    cpu_init(&cpu, &bus);
    cpu_reset(&cpu);
    cpu.pc = 0xC000; // hack, only for the testrom!!!
    cpu_run(&cpu, dump_cpu);
    
    
    return 0; 
}






