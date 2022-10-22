#include "conv.h"
#include "divmul32.h"

char itoa_buf[32];

char* itoa(uint32_t num, char* buffer, uint8_t base) {
    
    char* ptr_a = buffer;
    char* ptr_b = buffer;
    
    if(!num) {
        
        buffer[0] = '0';
        buffer[1] = 0;
        return buffer;
        
    }
    
    for(; num; num = divide32(num, base)) {
        
        *ptr_a = "0123456789ABCDEF"[remainder32(num, base)];
        ptr_a++;
        
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

size_t get_number_characters(char* str, uint8_t base) {
    
    char* end = str;
    
    for(; 1; end++) {
        
        uint8_t digit;
        if(*end >= '0' && *end <= '9') digit = *end - '0';
        else if(*end >= 'a' && *end <= 'f') digit = *end - 'a';
        else if(*end >= 'A' && *end <= 'F') digit = *end - 'A';
        else break;
        
        if(digit >= base) break;
        
    }
    
    end--;
    return (size_t)end - (size_t)str + 1;
    
}

uint32_t atoi(char* str, uint8_t base) {
    
    char* end = str + get_number_characters(str, base) - 1;
    uint32_t weight = 1;
    uint32_t ret = 0;
    
    for(; end >= str; --end) {
        
        if(*end >= '0' && *end <= '9') ret += multiply32(*end - '0', weight);
        else if(*end >= 'a' && *end <= 'f') ret += multiply32(*end - 'a' + 0xa, weight);
        else if(*end >= 'A' && *end <= 'F') ret += multiply32(*end - 'A' + 0xa, weight);
        weight = multiply32(weight, base);
        
    }
    
    return ret;
    
}
