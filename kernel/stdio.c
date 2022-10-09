#include "stdio.h"
#include "intr.h"

void putc(char ch) {
    
    int10h_teletype_output(ch, 0, 0);
    
}

void puts(const char* str) {
    
    while(*str) putc(*str++);
    
}
