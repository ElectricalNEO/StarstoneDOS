#include "memory.h"

void memcpy(void* src, void* dest, size_t size) {
    
    size_t i = 0;
    for(; i < size; i++) {
        
        *((uint8_t*)dest + i) = *((uint8_t*)src + i);
        
    }
    
}

int8_t memcmp(void* ptr1, void* ptr2, size_t size) {
    
    size_t i = 0;
    for(; i < size; i++) {
        
        if(*((uint8_t*)ptr1 + i) > *((uint8_t*)ptr2 + i)) return 1;
        else if(*((uint8_t*)ptr1 + i) < *((uint8_t*)ptr2 + i)) return -1;
        
    }
    
    return 0;
    
}

void memset(void* mem, uint8_t val, size_t size) {
    
    size_t i = 0;
    for(; i < size; i++) *((uint8_t*)mem + i) = val;
    
}
