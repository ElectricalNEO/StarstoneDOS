#include "stdio.h"
#include "itoa.h"

char* itoa_buf = "32767";

void start(uint8_t drive) {
    
    puts("Starstone 1.0\r\n");
    puts(itoa(drive, itoa_buf, 16));
    
    while(1);
    
}
