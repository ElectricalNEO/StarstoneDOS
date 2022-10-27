#include "stdio.h"
#include "conv.h"

void putc(char ch);

void puts(const char* str) {
    
    while(*str && *str != '$') putc(*str++);
    
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
                    puts(itoa(*(uint32_t*)arg_p, itoa_buf, 10));
                    arg_p+=2;
                    break;
                case 'x':
                    puts(itoa((uint16_t)*arg_p, itoa_buf, 16));
                    arg_p++;
                    break;
                case 'X':
                    puts(itoa(*(uint32_t*)arg_p, itoa_buf, 16));
                    arg_p+=2;
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

void gets(char* buf, size_t buf_size) {
    
    size_t i = 0;
    while(1) {
        
        char key = getc();
        
        if(key == '\b' && i > 0) {
            
            puts("\b \b");
            i--;
            
        } else if(key == '\r') {
            
            putc('\r');
            putc('\n');
            buf[i] = 0;
            return;
            
        } else if(i < buf_size - 1 && key != '\b') {
            
            putc(key);
            buf[i] = key;
            i++;
            
        }
        
    }
    
}
