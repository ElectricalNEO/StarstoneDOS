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
                    puts(itoa(*arg_p, itoa_buf, 10));
                    arg_p++;
                    break;
                case 'x':
                    puts(itoa(*arg_p, itoa_buf, 16));
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
