#include "memory.h"

void memcpy(void* src, void* dest, size_t size) {
    
    size_t i = 0;
    for(; i < size; i++) {
        
        *((uint8_t*)dest + i) = *((uint8_t*)src + i);
        
    }
    
}
