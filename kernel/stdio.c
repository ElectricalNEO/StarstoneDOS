#include "stdio.h"
#include "conv.h"

#pragma aux asm_putc = \
"mov ah, 0eh" \
"xor bx, bx" \
"int 10h" \
modify [ah bx] \
parm [al]

void asm_putc(char ch);

void putc(char ch) {
    
    if(ch < 27 && ch != '\r' && ch != '\n' && ch != '\b') { // CTRL+
        
        asm_putc('^');
        asm_putc('A' + ch - 1);
        
    } else asm_putc(ch);
    
}

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
            
            if(buf[i-1] < 27 && buf[i-1] != '\r' && buf[i-1] != '\n' && buf[i-1] != '\b') puts("\b\b  \b\b");
            else puts("\b \b");
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
