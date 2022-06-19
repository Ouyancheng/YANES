#include "sdk.h"

int panic_at(const char *file, int line, const char *function, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int n = 0; 
    n += printf("At file: %s:%d function %s...\nPANIC: ", file, line, function); 
    n += vprintf(format, args); 
    va_end(args); 
    exit(1); 
    return n; 
}

uint8_t *load_file(const char *filepath, size_t *datasize) {
    FILE *romfile = fopen(filepath, "rb"); 
    if (!romfile) {
        printf("romfile not found!\n"); 
        return NULL;
    }
    size_t romsize; 
    fseek(romfile, 0L, SEEK_END);
    romsize = ftell(romfile);

    rewind(romfile); 

    uint8_t *data = (uint8_t*)allocate_memory(romsize);

    if (fread(data, romsize, 1, romfile) != 1) {
        printf("rom file read failed!\n"); 
        deallocate_memory(data);
        fclose(romfile);
        return NULL;
    }
    fclose(romfile);
    *datasize = romsize;
    return data;
}
void deallocate_filedata(uint8_t *data) {
    deallocate_memory(data);
}
void *allocate_memory(size_t size) {
    return malloc(size);
}

void deallocate_memory(void *ptr) {
    free(ptr); 
}

