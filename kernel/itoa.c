#include "itoa.h"

char itoa_buf[6];

char* itoa(size_t num, char* buffer, uint8_t base) {
    
    char* ptr_a = buffer;
    char* ptr_b = buffer;
    
    if(!num) {
        
        buffer[0] = '0';
        buffer[1] = 0;
        return buffer;
        
    }
    
    for(; num; num /= base, ptr_a++) {
        
        *ptr_a = "0123456789ABCDEF"[num % base];
        
    }
    
    *ptr_a = 0;
    ptr_a--;
    
    for(; ptr_b < ptr_a; ptr_b++, ptr_a--) {
        
        char ch = *ptr_a;
        *ptr_a = *ptr_b;
        *ptr_b = ch;
        
    }
    
    return buffer;
    
}
