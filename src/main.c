#include "cpu.h"
#include "cpu_opcodes.h"
int main(int argc, const char *const argv[]) {
    printf("sizeof cpu_opcode=%lu sizeof opcode_table=%lu\n", sizeof(struct cpu_opcode), sizeof(opcode_table)); 
    return 0; 
}






