#include "stdio.h"
#include "drive.h"
#include "heap.h"

drive_t boot_disk;

char disk_read_buffer[512];

extern char _BREAK;

void start(uint8_t drive) {
    
    uint8_t i;
    uint8_t drives_connected;
    drive_t* drives;
    
    clear();
    set_attributes(0x0f);
    
    puts("Starstone 1.0\r\n");
    
    if(init_drive(&boot_disk, drive)) {
        
        printf("Failed to get drive 0x%x parameters!\r\nHalting...", drive);
        while(1);
        
    }
    
    init_heap(&_BREAK);
    
    drives = init_drives(&drives_connected);
    
    puts("Initialization complete.\r\n");
    
    for(i = 0; i < drives_connected; i++) {
        
        printf("DRIVE: 0x%x\r\n", drives[i].drive_num);
        
    }
    
    while(1);
    
}
