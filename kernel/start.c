#include "stdio.h"
#include "itoa.h"
#include "drive.h"

drive_t boot_disk;

uint16_t cylinder_sector;
uint8_t head;

char disk_read_buffer[512];

void start(uint8_t drive) {
    
    puts("Starstone 1.0\r\n");
    
    if(init_drive(&boot_disk, drive)) {
        
        puts("Failed to get drive 0x");
        puts(itoa(drive, itoa_buf, 16));
        puts(" parameters!\r\nHalting...");
        while(1);
        
    }
    
    if(drive_read_sector_x(&boot_disk, 0, 0, (uint16_t)disk_read_buffer, 5)) {
        
        puts("\r\nFailed to read 1 sector starting from LBA 0!\r\nHalting...");
        while(1);
        
    }
    
    puts("Done!\r\nBuffer address: 0x");
    puts(itoa((size_t)disk_read_buffer, itoa_buf, 16));
    
    while(1);
    
}
