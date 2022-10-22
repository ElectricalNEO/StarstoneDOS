#include "file.h"
#include "string.h"
#include "fs_manager.h"
#include "heap.h"
#include "memory.h"

typedef struct file_t {
    
    fat12_16_t* fs;
    fat_dir_entry_t dir_entry;
    uint32_t seek;
    
} file_t;

file_t* fopen(char* path) {
    
    fat12_16_t* fs = 0;
    char* token = strtok(path, "/\\");
    file_t* dir;
    char filename[12] = {0};
    
    if(strlen(token)) { // partition specified
        
        fs = get_fs_by_part_name(token);
        
    } else { // use the boot partition
        
        fs = get_fs_by_part_name("boot");
        
    }
    
    if(!fs) return 0;
    
    token = strtok(0, "/\\");
    
    dir = (file_t*)kmalloc(sizeof(file_t));
    dir->dir_entry.cluster_low = 0;
    dir->fs = fs;
    
    while(token) {
        
        uint8_t i = strlen(token) - 1;
        uint8_t name_len = 0;
        
        memset(filename, ' ', 11);
        
        for(; i; i--) {
            
            if(token[i] == '.') {
                
                uint8_t ext_len = strlen(&token[i + 1]);
                if(ext_len > 3) ext_len = 3;
                memcpy(&token[i + 1], &filename[8], ext_len);
                
                name_len = i;
                
            }
            
        }
        
        if(!name_len) name_len = strlen(token);
        if(name_len > 8) name_len = 8;
        
        memcpy(token, filename, name_len);
        
        to_uppercase(filename);
        
        if(!dir->dir_entry.cluster_low) dir->dir_entry = fat12_16_find_in_root_dir(fs, filename);
        else dir->dir_entry = fat12_16_find_in_dir(fs, filename, &dir->dir_entry);
        
        if(!dir->dir_entry.cluster_low) {
            
            kfree(dir);
            return 0;
            
        }
        
        token = strtok(0, "/\\");
        
    }
    
    return dir;
    
}

void fclose(file_t* file) {
    
    kfree(file);
    
}

int8_t fseek(file_t* file, uint32_t offset, uint8_t origin) {
    
    switch(origin) {
        
        case SEEK_SET:
            if(offset >= file->dir_entry.size) return 1;
            file->seek = offset;
            break;
        case SEEK_CUR:
            if(offset + file->seek >= file->dir_entry.size) return 1;
            file->seek += offset;
            break;
        case SEEK_END:
            if(offset > file->dir_entry.size) return 1;
            file->seek = file->dir_entry.size - offset;
            break;
        default:
            return 1;
        
    }
    
    return 0;
    
}

uint32_t ftell(file_t* file) {
    
    return file->seek;
    
}

uint8_t fread(file_t* file, uint16_t segment, uint16_t offset, uint32_t size) {
    
    return fat12_16_read_file_bytes(file->fs, &file->dir_entry, file->seek, size, segment, offset);
    
}
