#include "cpu.h"
#include "cpu_instruction.h"
#include "cpu_addressmode.h"
int main(int argc, const char *const argv[]) {
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
    struct nescpu cpu;
    bool dummy;
    printf("test compute addr %d\n", cpu_compute_address(&cpu, MODE_IMP, 0, &dummy));
    // cpu_init(&cpu);
    // cpu_reset(&cpu);
    cpu_run(&cpu, NULL);
    
    
    return 0; 
}






