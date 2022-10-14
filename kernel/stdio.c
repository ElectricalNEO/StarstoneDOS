#include "stdio.h"

void putc(char ch);

void puts(const char* str) {
    
    while(*str) putc(*str++);
    
}
