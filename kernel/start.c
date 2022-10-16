#include "stdio.h"
#include "drive.h"

drive_t boot_disk;

char disk_read_buffer[512];

extern char _BREAK;

void start(uint8_t drive) {
    
    puts("Starstone 1.0\r\n");
    
    if(init_drive(&boot_disk, drive)) {
        
        printf("Failed to get drive 0x%x parameters!\r\nHalting...", drive);
        while(1);
        
    }
    
    puts("Initialization complete.\r\n");
    
    printf("Program break: 0x%x\r\n", (size_t)&_BREAK);
    
    while(1);
    
}
