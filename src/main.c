#include "cpu.h"
#include "cpu_opcodes.h"
int main(int argc, const char *const argv[]) {
    printf("sizeof cpu_opcode=%lu sizeof opcode_table=%lu, sizeof cpu_instruction=%lu, sizeof addressmode=%lu\n", 
        sizeof(struct cpu_opcode), sizeof(opcode_table),
        sizeof(enum cpu_instruction),
        sizeof(enum addressmode)
    ); 
    print_cpu_opcode_nametable(); 
    return 0; 
}






