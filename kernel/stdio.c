#include "stdio.h"

void putc(char ch);
#pragma aux putc = \
"mov ah, 0eh" \
"xor bx, bx" \
"int 10h" \
modify [ah bx] \
parm [al]

void puts(const char* str) {
    
    while(*str) putc(*str++);
    
}
