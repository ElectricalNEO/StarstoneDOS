#include "fs_manager.h"
#include "partition_manager.h"
#include "fs.h"
#include "heap.h"
#include "memory.h"
#include "conv.h"
#include "stdio.h"

fs_list_t* fs_list = 0;

void init_fs_manager() {
    
    partition_list_t* partitions;
    partition_list_t* partitions_node;
    fs_list_t* fs_node;
    
    if(fs_list) return;
    
    fs_list = kmalloc(sizeof(fs_list_t));
    fs_node = fs_list;
    
    partitions = get_partitions();
    
    for(partitions_node = partitions; partitions_node->next; partitions_node = partitions_node->next) {
        
        if(!init_fs(&fs_node->data, &partitions_node->data)) {
            
            fs_node->next = kmalloc(sizeof(fs_list_t));
            fs_node = fs_node->next;
            
        }
        
    }
    
    kfree(fs_node->next);
    fs_node->next = 0;
    
}

fs_t* get_fs_by_part_id(uint8_t drive_id, uint8_t part_id) {
    
    fs_list_t* fs_node;
    
    if(!fs_list) return 0;
    for(fs_node = fs_list; fs_node->next; fs_node = fs_node->next) {
        
        if(fs_node->data.partition.drive.drive_num == drive_id && fs_node->data.partition.part_num == part_id) return &fs_node->data;
        
    }
    
    return 0;
    
}

fs_t* get_fs_by_part_name(char* name) {
    
    uint8_t drive_id;
    uint8_t part_id;
    
    if(!memcmp(name, "boot", 4)) {
        
        partition_t* part = get_partition_by_name("boot");
        if(!part) return 0;
        
        return get_fs_by_part_id(part->drive.drive_num, part->part_num);
        
    }
    
    if(!memcmp(name, "hd", 2)) drive_id = 0x80;
    else if(!memcmp(name, "fd", 2)) drive_id = 0;
    else return 0;
    
    name += 2;
    drive_id += atoi(name, 16);
    
    name += get_number_characters(name, 16);
    name++;
    part_id = atoi(name, 10);
    
    return get_fs_by_part_id(drive_id, part_id);
    
}

fs_list_t* get_all_fs() {
    
    return fs_list;
    
}
