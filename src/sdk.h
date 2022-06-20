#pragma once 

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int panic_at(const char *file, int line, const char *function, const char *format, ...); 
#define panic(...) panic_at(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef __GNUC__
#define PACKED __attribute__((__packed__))
#else 
#define PACKED 
#endif 

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

/// NOTE: returns allocated memory, remember to free
uint8_t *load_file(const char *filepath, size_t *datasize); 
void deallocate_filedata(uint8_t *data); 
void *allocate_memory(size_t size);
void deallocate_memory(void *ptr);

inline uint8_t set_mask(uint8_t reg, uint8_t mask, bool val) {
    return (val) ? (reg | mask) : (reg & (~mask));
}


