#include "cpu.h"
#include "cpu_instruction.h"
#include "cpu_addressmode.h"
#include "system.h"

void test_snake(void);

struct nessystem sys;

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

    size_t datasize = 0;
    uint8_t *rom_data = load_file("../testroms/nestest.nes", &datasize);
    int ret;
    system_init(&sys);
    ret = system_load_rom_ines(&sys, rom_data, datasize);
    if (ret < 0) {
        panic("failed to load rom\n"); 
    }
    system_test_run(&sys, 0xC000, dump_cpu);
    system_unload_rom(&sys);

    // test_snake();
    
    return 0; 
}






