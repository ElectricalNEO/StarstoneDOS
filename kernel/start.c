#include "stdio.h"
#include "itoa.h"

char* itoa_buf = "32767";

void start() {
    
    
    puts("Starstone 1.0\r\n");
    puts(itoa(123, itoa_buf, 10));
    
    while(1);
    
}
