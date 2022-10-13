#include "stdio.h"
#include "itoa.h"
#include "drive.h"

char* itoa_buf = "32767";
drive_t boot_disk;

void start(uint8_t drive) {
    
    puts("Starstone 1.0\r\n");
    
    if(init_drive(&boot_disk, drive)) {
        
        puts("Failed to get drive 0x");
        puts(itoa(drive, itoa_buf, 16));
        puts(" parameters!\r\nHalting...");
        while(1);
        
    }
    
    puts("Drive number: ");
    puts(itoa(boot_disk.drive_num, itoa_buf, 10));
    puts("\r\nSectors per track: ");
    puts(itoa(boot_disk.sectors_per_track, itoa_buf, 10));
    puts("\r\nHeads: ");
    puts(itoa(boot_disk.heads, itoa_buf, 10));
    
    while(1);
    
}
