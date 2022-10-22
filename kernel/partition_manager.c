#include "partition_manager.h"
#include "drive_manager.h"
#include "conv.h"
#include "memory.h"

partition_list_t* partitions = 0;
partition_t* boot_part = 0;

void init_partition_manager(char boot_drive) {
    
    uint8_t i = 0;
    
    if(partitions) return;
    partitions = init_partitions(get_drives());
    
    for(; i < 4; i++) {
        
        boot_part = get_partition_by_id(boot_drive, i);
        if(boot_part && boot_part->bootable) break;
        
    }
    
}

partition_t* get_partition_by_id(uint8_t drive_id, uint8_t part_id) {
    
    partition_list_t* partitions_node;
    
    if(!partitions) return 0;
    for(partitions_node = partitions; partitions_node->next; partitions_node = partitions_node->next) {
        
        if(partitions_node->data.drive.drive_num == drive_id && partitions_node->data.part_num == part_id) return &partitions_node->data;
        
    }
    
    return 0;
    
}

partition_t* get_partition_by_name(char* name) {
    
    uint8_t drive_id = 0;
    uint8_t part_id;
    
    if(!memcmp(name, "boot", 4)) return boot_part;
    
    if(!memcmp(name, "hd", 2)) drive_id = 0x80;
    else if(!memcmp(name, "fd", 2)) drive_id = 0;
    else return 0;
    
    name += 2;
    drive_id += atoi(name, 16);
    
    name += get_number_characters(name, 16) + 1;
    part_id = atoi(name, 10);
    
    return get_partition_by_id(drive_id, part_id);
    
}

partition_list_t* get_partitions() {
    
    return partitions;
    
}
