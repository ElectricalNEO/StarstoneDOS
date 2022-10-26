#include "stdio.h"
#include "heap.h"
#include "drive_manager.h"
#include "partition_manager.h"
#include "fs_manager.h"
#include "shell.h"
#include "memory.h"

void start(uint8_t drive) {
    
    fs_t* ext2;
    ext2_inode_t inode;
    
    clear();
    
    printf("Starstone 1.0\r\n");
    
    init_heap();
    init_drive_manager();
    init_partition_manager(drive);
    init_fs_manager();
    
    printf("Initialization complete.\r\n");
    
    ext2 = get_fs_by_part_name("hd0p1");
    if(!ext2) {
        
        printf("No hd0p1!\r\n");
        while(1);
        
    }
    
    if(ext2_read_inode(ext2, 2, &inode)) {
        
        printf("Failed to read root directory inode!\r\n");
        
    } else {
        
        if(!ext2_find_file(ext2, &inode, &inode, "file123123")) {
            
            printf("File found!\r\n");
            
        }
        
    }
    
    {
        
        char command[255];
        
        printf("\r\nWelcome to Starstone Built-in Shell! Type \"help\" to get a list of commands\r\n");
        
        shell_dir_path = kmalloc(5);
        memcpy("boot", shell_dir_path, 5);
        
        while(1) {
            
            printf("%s >>> ", shell_dir_path);
            gets(command, 255);
            exec_command(command);
            
        }
        
    }
    
    while(1);
    
}
