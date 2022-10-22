#pragma once

#include "types.h"

typedef struct memory_chunk {
    
    struct memory_chunk* prev;
    struct memory_chunk* next;
    uint8_t used;
    
} memory_chunk_t;

extern char _BREAK;

void init_heap();
void* kmalloc(size_t size);
void kfree(void* addr);
