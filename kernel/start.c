#include "stdio.h"
#include "drive.h"
#include "heap.h"

drive_t boot_disk;

char disk_read_buffer[512];

extern char _BREAK;

void start(uint8_t drive) {
    
    void* mem;
    void* mem2;
    
    puts("Starstone 1.0\r\n");
    
    if(init_drive(&boot_disk, drive)) {
        
        printf("Failed to get drive 0x%x parameters!\r\nHalting...", drive);
        while(1);
        
    }
    
    init_heap(&_BREAK);
    
    puts("Initialization complete.\r\n");
    
    printf("Program break: 0x%x\r\n", (size_t)&_BREAK);
    
    mem2 = kmalloc(0x1000);
    printf("Allocated memory: 0x%x\r\n", mem2);
    mem = kmalloc(0x102);
    printf("Allocated memory: 0x%x\r\n", mem);
    kfree(mem2);
    mem = kmalloc(0x2);
    printf("Allocated memory: 0x%x\r\n", mem);
    mem = kmalloc(0x18);
    printf("Allocated memory: 0x%x\r\n", mem);
    mem = kmalloc(0x14);
    printf("Allocated memory: 0x%x\r\n", mem);
    print_heap();
    printf("Memory chunk size: 0x%x\r\n", sizeof(memory_chunk_t));
    
    while(1);
    
}
