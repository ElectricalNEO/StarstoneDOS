#include "file.h"
#include "string.h"
#include "fs_manager.h"
#include "heap.h"
#include "memory.h"

typedef union file_fs_t {
    
    fat_dir_entry_t fat;
    ext2_inode_t ext2;
    
} file_fs_t;

typedef struct file_t {
    
    fs_t* fs;
    file_fs_t file;
    uint32_t seek;
    
} file_t;

file_t* fopen(char* path) {
    
    fs_t* fs = 0;
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
    
    if(!token) return 0;
    
    dir = (file_t*)kmalloc(sizeof(file_t));
    
    switch(fs->type) {
        
        case FAT12:
        case FAT16:
            dir->file.fat.cluster_low = 0;
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
                
                if(!dir->file.fat.cluster_low) dir->file.fat = fat12_16_find_in_root_dir(fs, filename);
                else dir->file.fat = fat12_16_find_in_dir(fs, filename, &dir->file.fat);
                
                if(!dir->file.fat.cluster_low) {
                    
                    kfree(dir);
                    return 0;
                    
                }
                
                token = strtok(0, "/\\");
                
            }
            
            return dir;
        
        default:
            
            return 0;
    }
    
}

void fclose(file_t* file) {
    
    kfree(file);
    
}

int8_t fseek(file_t* file, uint32_t offset, uint8_t origin) {
    
    switch(origin) {
        
        case SEEK_SET:
            if(offset >= file->file.fat.size) return 1;
            file->seek = offset;
            break;
        case SEEK_CUR:
            if(offset + file->seek >= file->file.fat.size) return 1;
            file->seek += offset;
            break;
        case SEEK_END:
            if(offset > file->file.fat.size) return 1;
            file->seek = file->file.fat.size - offset;
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
    
    switch(file->fs->type) {
        
        case FAT12:
        case FAT16:
            return fat12_16_read_file_bytes(file->fs, &file->file.fat, file->seek, size, segment, offset);
        
        default:
            return 1;
        
    }
    
}

uint8_t get_dir_entry(char* dir, size_t entry_index, char* buf) {
    
    char filename[11];
    uint8_t i = 0;
    fs_t* fs = get_fs_by_part_name(dir);
    uint8_t is_root_dir = 1;
    
    for(i = 0; dir[i]; i++) {
        
        if(*(dir + i - 1) == '/' || *(dir + i - 1) == '\\') {
            
            char* dir_path = kmalloc(strlen(dir) + 1);
            file_t* directory;
            
            memcpy(dir, dir_path, strlen(dir) + 1);
            directory = fopen(dir_path);
            kfree(dir_path);
            
            if(!directory) return 1;
            
            directory->seek = entry_index * sizeof(fat_dir_entry_t);
            if(fread(directory, 0, (uint16_t)filename, 11)) return 1;
            
            is_root_dir = 0;
            
            fclose(directory);
            
        }
        
    }
    
    switch(fs->type) {
        
        case FAT12:
        case FAT16:
            if(is_root_dir) {
                
                if(!fs) return 1;
                
                if(entry_index >= fs->fs.fat12_16.root_dir_entries) return 1;
                
                if(partition_read_sector(&fs->partition, fs->fs.fat12_16.root_dir_start + entry_index / 16, 0, (uint16_t)disk_tmp_buffer)) {
                    
                    return 1;
                    
                }
                
                memcpy((void*)((uint16_t)disk_tmp_buffer + 32 * (entry_index % 16)), filename, 11);
                
            }
            
            memcpy(filename, buf, 8);
            
            for(i = 0; i < 8; i++) if(filename[i] == ' ') break;
            
            if(filename[8] == ' ') {
                
                buf[i + 1] = 0;
                return 0;
                
            }
            
            buf[i] = '.';
            
            memcpy(filename + 8, buf + i + 1, 3);
            
            i++;
            if(buf[i] == ' ') buf[i] = 0;
            else if(buf[i+1] == ' ') buf[i+1] = 0;
            else if(buf[i+2] == ' ') buf[i+2] = 0;
            else buf[i+3] = 0;
            
            return 0;
        
        default:
            return 1;
        
    }
    
    
    
}

uint8_t is_dir(file_t* file) {
    
    switch(file->fs->type) {
        
        case FAT12:
        case FAT16:
            return file->file.fat.attributes & 0x10;
        
        default:
            return 1;
        
    }
    
}
