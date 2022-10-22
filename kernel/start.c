#include "stdio.h"
#include "heap.h"
#include "drive_manager.h"
#include "partition_manager.h"
#include "fs_manager.h"
#include "shell.h"
#include "memory.h"

void start(uint8_t drive) {
    
    clear();
    
    printf("Starstone 1.0\r\n");
    
    init_heap();
    init_drive_manager();
    init_partition_manager(drive);
    init_fs_manager();
    
    printf("Initialization complete.\r\n");
    
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
