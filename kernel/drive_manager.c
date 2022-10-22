#include "drive_manager.h"
#include "memory.h"
#include "conv.h"

drive_list_t* drives = 0;

void init_drive_manager() {
    
    if(drives) return;
    drives = init_drives();
    
}

drive_t* get_drive_by_id(uint8_t id) {
    
    drive_list_t* drives_node;
    
    if(!drives) return 0;
    for(drives_node = drives; drives_node->next; drives_node = drives_node->next) {
        
        if(drives_node->data.drive_num == id) return &drives_node->data;
        
    }
    
    return 0;
    
}

drive_t* get_drive_by_name(char* name) {
    
    uint8_t id;
    if(!memcmp(name, "hd", 2)) id = 0x80;
    else if(!memcmp(name, "fd", 2)) id = 0;
    else return 0;
    
    id += atoi(&name[2], 16);
    
    return get_drive_by_id(id);
    
}

drive_list_t* get_drives() {
    
    return drives;
    
}
