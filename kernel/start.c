#include "stdio.h"
#include "heap.h"
#include "drive_manager.h"
#include "partition_manager.h"
#include "fs_manager.h"

void start(uint8_t drive) {
    
    fat12_16_t* boot_fs;
    fat_dir_entry_t file;
    char rhoncus[8];
    
    clear();
    set_attributes(0x0f);
    
    printf("Starstone 1.0\r\n");
    
    init_heap();
    init_drive_manager();
    init_partition_manager();
    init_fs_manager();
    
    {uint8_t i = 0; for(; i < 4; i++) {
        
        boot_fs = get_fs_by_part_id(drive, i);
        if(boot_fs && boot_fs->partition.bootable) break;
        boot_fs = 0;
        
    }}
    
    if(!boot_fs) {
        
        printf("Failed to initialize file system on the boot partition!\r\nHalting...\r\n");
        while(1);
        
    }
    
    printf("Initialization complete.\r\n");
    
    file = fat12_16_find_in_root_dir(boot_fs, "LOREMIPSTXT");
    if(!file.cluster_low) {
        
        printf("File \"LOREMIPS.TXT\" not found!\r\nHalting...\r\n");
        while(1);
        
    }
    
    if(fat12_16_read_file_bytes(boot_fs, &file, 0x1ff, 7, 0, (size_t)rhoncus)) {
        
        printf("Failed to read 7 bytes starting at 511th byte from file \"LOREMIPS.TXT\"!\r\nHalting...\r\n");
        
    }
    
    rhoncus[7] = 0;
    printf(rhoncus);
    
    while(1);
    
}
