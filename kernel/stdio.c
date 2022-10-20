#include "stdio.h"
#include "itoa.h"

void putc(char ch);

void puts(const char* str) {
    
    while(*str) putc(*str++);
    
}

void printf(const char* fmt, ...) {
    
    size_t* arg_p = (size_t*)&fmt;
    
    arg_p++;
    
    while(*fmt) {
        
        if(*fmt != '%') {
            
            putc(*fmt);
            fmt++;
            
        } else {
            
            fmt++;
            
            switch(*fmt) {
                
                case 'd':
                    puts(itoa((uint16_t)*arg_p, itoa_buf, 10));
                    arg_p++;
                    break;
                case 'D':
                    puts(itoa((uint32_t)*arg_p, itoa_buf, 10));
                    arg_p+=2;
                    break;
                case 'x':
                    puts(itoa((uint16_t)*arg_p, itoa_buf, 16));
                    arg_p++;
                    break;
                case 's':
                    puts((char*)*arg_p);
                    arg_p++;
                    break;
                case 'c':
                    putc((char)*arg_p);
                    arg_p++;
                    break;
                default:
                    putc('%');
                
            }
            
            fmt++;
            
        }
        
    }
    
}
