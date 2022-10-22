#include "stdio.h"
#include "heap.h"
#include "drive_manager.h"
#include "partition_manager.h"
#include "fs_manager.h"
#include "file.h"
#include "string.h"

void start(uint8_t drive) {
    
    file_t* file;
    char rhoncus[8];
    char name[20];
    
    clear();
    set_attributes(0x0f);
    
    printf("Starstone 1.0\r\n");
    
    init_heap();
    init_drive_manager();
    init_partition_manager(drive);
    init_fs_manager();
    
    printf("Initialization complete.\r\n");
    
    printf("Enter your name: ");
    gets(name, 20);
    printf("Hello, %s!\r\n", name);
    
    file = fopen("/data/loremips.txt");
    
    if(!file) {
        
        printf("Failed to open file \"/DATA/LOREMIPS.TXT!\"!\r\nHalting...\r\n");
        while(1);
        
    }
    
    if(fseek(file, 0x1ff, SEEK_SET)) {
        
        printf("Failed to seek to byte 511th of file \"/DATA/LOREMIPS.TXT!\"!\r\nHalting...\r\n");
        
    }
    
    if(fread(file, 0, (uint16_t)rhoncus, 7)) {
        
        printf("Failed to read 7 bytes starting at byte 511th from file \"/DATA/LOREMIPS.TXT\"!\r\nHalting...\r\n");
        
    }
    
    rhoncus[7] = 0;
    printf(rhoncus);
    
    while(1);
    
}
