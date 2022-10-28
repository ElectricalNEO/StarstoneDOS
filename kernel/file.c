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
    uint32_t size;
    uint8_t is_dir;
    
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
    dir->fs = fs;
    dir->seek = 0;
    
    switch(fs->type) {
        
        case FAT12:
        case FAT16:
            dir->file.fat.cluster_low = 0;
            
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
            
            dir->size = dir->file.fat.size;
            dir->is_dir = dir->file.fat.attributes & 0x10;
            return dir;
            
        case EXT2:
            
            if(ext2_read_inode(fs, 2, &dir->file.ext2)) {
                
                kfree(dir);
                return 0;
                
            }
            
            while(token) {
                
                if(ext2_find_file(fs, &dir->file.ext2, &dir->file.ext2, token)) {
                    
                    kfree(dir);
                    return 0;
                    
                }
                
                token = strtok(0, "/\\");
                
            }
            
            dir->size = dir->file.ext2.size_lower;
            dir->is_dir = (dir->file.ext2.type_permissions & 0x4000) > 0 ? 1 : 0;
            return dir;
            
        default:
            
            kfree(dir);
            return 0;
    }
    
}

void fclose(file_t* file) {
    
    kfree(file);
    
}

int8_t fseek(file_t* file, uint32_t offset, uint8_t origin) {
    
    switch(origin) {
        
        case SEEK_SET:
            if(offset >= file->size) return 1;
            file->seek = offset;
            break;
        case SEEK_CUR:
            if(offset + file->seek >= file->size) return 1;
            file->seek += offset;
            break;
        case SEEK_END:
            if(offset > file->size) return 1;
            file->seek = file->size - offset;
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
        
        case EXT2:
            return ext2_read_file_bytes(file->fs, &file->file.ext2, file->seek, size, segment, offset);
        
        default:
            return 1;
        
    }
    
}

char* get_dir_entry(char* dir, size_t entry_index) {
    
    char filename[11];
    char* buf;
    uint8_t i = 0;
    fs_t* fs = get_fs_by_part_name(dir);
    uint8_t is_root_dir = 1;
    file_t* directory;
    
    for(i = 0; dir[i]; i++) {
        
        if(*(dir + i - 1) == '/' || *(dir + i - 1) == '\\') {
            
            char* dir_path = kmalloc(strlen(dir) + 1);
            
            memcpy(dir, dir_path, strlen(dir) + 1);
            directory = fopen(dir_path);
            kfree(dir_path);
            
            if(!directory) return 0;
            
            is_root_dir = 0;
            
        }
        
    }
    
    switch(fs->type) {
        
        case FAT12:
        case FAT16:
            
            buf = kmalloc(13);
            
            if(is_root_dir) {
                
                if(!fs || entry_index >= fs->fs.fat12_16.root_dir_entries) {
                    
                    kfree(buf);
                    return 0;
                    
                }
                
                if(partition_read_sector(&fs->partition, fs->fs.fat12_16.root_dir_start + entry_index / 16, 0, (uint16_t)disk_tmp_buffer)) {
                    
                    kfree(buf);
                    return 0;
                    
                }
                
                memcpy((void*)((uint16_t)disk_tmp_buffer + 32 * (entry_index % 16)), filename, 11);
                
            } else {
                
                directory->seek = entry_index * sizeof(fat_dir_entry_t);
                if(fread(directory, 0, (uint16_t)filename, 11)) {
                    
                    fclose(directory);
                    return 0;
                    
                }
                
            }
            
            memcpy(filename, buf, 8);
            
            for(i = 0; i < 8; i++) if(filename[i] == ' ') break;
            
            if(filename[8] == ' ') {
                
                buf[i + 1] = 0;
                return buf;
                
            }
            
            buf[i] = '.';
            
            memcpy(filename + 8, buf + i + 1, 3);
            
            i++;
            if(buf[i] == ' ') buf[i] = 0;
            else if(buf[i+1] == ' ') buf[i+1] = 0;
            else if(buf[i+2] == ' ') buf[i+2] = 0;
            else buf[i+3] = 0;
            
            return buf;
        
        case EXT2:
            {
                uint32_t dir_entry_offset = 0;
                
                if(is_root_dir) {
                    
                    directory = kmalloc(sizeof(file_t));
                    if(ext2_read_inode(fs, 2, &directory->file.ext2)) {
                        
                        kfree(directory);
                        return 0;
                        
                    }
                    
                }
                
                i = 0;
                
                while(1) {
                    
                    if(i == entry_index) {
                        
                        uint8_t name_len;
                        
                        if(ext2_read_file_bytes(fs, &directory->file.ext2, dir_entry_offset + 6, 1, 0, (uint16_t)&name_len)) {
                            
                            fclose(directory);
                            return 0;
                            
                        }
                        
                        buf = kmalloc(name_len + 1);
                        
                        if(ext2_read_file_bytes(fs, &directory->file.ext2, dir_entry_offset + 8, name_len, 0, (uint16_t)buf)) {
                            
                            kfree(buf);
                            fclose(directory);
                            return 0;
                            
                        }
                        
                        buf[name_len] = 0;
                        fclose(directory);
                        return buf;
                        
                    } else {
                        
                        uint16_t size;
                        
                        if(ext2_read_file_bytes(fs, &directory->file.ext2, dir_entry_offset + 4, 2, 0, (uint16_t)&size)) {
                            
                            fclose(directory);
                            return 0;
                            
                        }
                        dir_entry_offset += size;
                        
                    }
                    
                    i++;
                    
                }
            }
        
        default:
            return 0;
        
    }
    
    
    
}

uint8_t is_dir(file_t* file) {
    
    return file->is_dir;
    
}
