#include "shell.h"
#include "string.h"
#include "stdio.h"
#include "file.h"
#include "string.h"
#include "heap.h"
#include "memory.h"
#include "fs_manager.h"
#include "drive_manager.h"
#include "partition_manager.h"

char* shell_dir_path;

void help() {
    
    printf("Commands:\r\n");
    printf("help - Show this list\r\n");
    printf("echo - Print text\r\n");
    printf("ls - Show contents of the current directory\r\n");
    printf("cd - Change directory\r\n");
    printf("cat - Print file's content\r\n");
    printf("clear - Clear screen\r\n");
    printf("part - Set current partition\r\n");
    printf("list - List disks [disk] or partitions [part]\r\n");
    
}

void echo() {
    
    char* token = strtok(0, "");
    if(token) puts(token);
    puts("\r\n");
    
}

void ls() {
    
    size_t entry = 0;
    char* filename;
    
    while(1) {
        
        filename = get_dir_entry(shell_dir_path, entry);
        if(!filename) return;
        
        if(filename[0] != 0) printf("%s\r\n", filename);
        entry++;
        kfree(filename);
        
    }
    
}

void cd() {
    
    char* token = strtok(0, "");
    char* path;
    file_t* dir;
    char* fpath;
    size_t i = strlen(shell_dir_path);
    
    if(!token) return;
    
    if(!strcmp(token, ".")) return;
    
    if(!strcmp(token, "..")) for(; i > 0; i--) if(shell_dir_path[i] == '/') {
        
        shell_dir_path[i] = 0;
        return;
        
    }
    
    path = kmalloc(strlen(shell_dir_path) + strlen(token) + 2);
    
    memcpy(shell_dir_path, path, strlen(shell_dir_path));
    
    if(token[0] == '/') memcpy(token, path + strlen(shell_dir_path), strlen(token) + 1);
    else memcpy(token, path + strlen(shell_dir_path) + 1, strlen(token) + 1);
    path[strlen(shell_dir_path)] = '/';
    
    fpath = kmalloc(strlen(path) + 1);
    memcpy(path, fpath, strlen(path) + 1);
    dir = fopen(fpath);
    kfree(fpath);
    
    if(!dir) {
        
        printf("No such directory: %s!\r\n", token);
        kfree(path);
        
        return;
        
    }
    
    if(!is_dir(dir)) {
        
        printf("%s is not a directory!\r\n", token);
        fclose(dir);
        kfree(path);
        
        return;
        
    }
    
    fclose(dir);
    
    kfree(shell_dir_path);
    shell_dir_path = path;
    
}

void cat() {
    
    char* token = strtok(0, "");
    char* path;
    file_t* file;
    uint32_t size;
    char* data;
    
    if(!token) return;
    
    path = kmalloc(strlen(shell_dir_path) + strlen(token) + 2);
    
    memcpy(shell_dir_path, path, strlen(shell_dir_path));
    
    if(token[0] == '/') memcpy(token, path + strlen(shell_dir_path), strlen(token) + 1);
    else memcpy(token, path + strlen(shell_dir_path) + 1, strlen(token) + 1);
    path[strlen(shell_dir_path)] = '/';
    
    file = fopen(path);
    kfree(path);
    
    if(!file) {
        
        printf("No such file: %s!\r\n", token);
        
        return;
        
    }
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if(size >= 80*23) {
        
        fseek(file, 80*23, SEEK_END);
        data = kmalloc(80*23 + 1);
        size = 80*23;
        
    }
    else data = kmalloc(size + 1);
    
    data[size] = 0;
    
    if(fread(file, 0, (uint16_t)data, size)) printf("Failed to read from that file!\r\n");
    else printf("%s\r\n", data);
    
    kfree(data);
    
    fclose(file);
    
}

void part() {
    
    char* token = strtok(0, "");
    fs_t* fs = get_fs_by_part_name(token);
    
    if(!token) return;
    
    if(!fs) printf("No recognized file system on partition %s!\r\n", token);
    else {
        
        char* path = kmalloc(strlen(token) + 1);
        kfree(shell_dir_path);
        memcpy(token, path, strlen(token) + 1);
        shell_dir_path = path;
        
    }
    
}

void list() {
    
    char* token = strtok(0, "");
    if(!token) return;
    
    if(!strcmp(token, "disk")) {
        
        drive_list_t* drives = get_drives();
        drive_list_t* node = drives;
        
        for(; node->next; node = node->next) {
            
            printf("%s%d\r\n", (node->data.drive_num >= 0x80) ? "hd" : "fd", node->data.drive_num % 0x80);
            
        }
        
    } else if(!strcmp(token, "part")) {
        
        fs_list_t* fs_list = get_all_fs();
        fs_list_t* node = fs_list;
        
        for(; node->next; node = node->next) {
            
            char* fs_type = "UNKNOWN";
            
            switch(node->data.type) {
                
                case FAT12:
                    fs_type = "FAT12";
                    break;
                case FAT16:
                    fs_type = "FAT16";
                    break;
                case EXT2:
                    fs_type = "EXT2";
                
            }
            
            printf("%s%dp%d%s [%s]\r\n", (node->data.partition.drive.drive_num >= 0x80) ? "hd" : "fd", node->data.partition.drive.drive_num % 0x80, node->data.partition.part_num, node->data.partition.bootable ? " [boot]" : "", fs_type);
            
        }
        
    } else {
        
        printf("Usage: list [disk/part]\r\n");
        
    }
    
}

void exec_command(char* cmd) {
    
    char* token = strtok(cmd, " ");
    
    if(!token) return;
    
    if(!strcmp(token, "help")) help();
    else if(!strcmp(token, "echo")) echo();
    else if(!strcmp(token, "ls")) ls();
    else if(!strcmp(token, "cd")) cd();
    else if(!strcmp(token, "cat")) cat();
    else if(!strcmp(token, "clear")) clear();
    else if(!strcmp(token, "part")) part();
    else if(!strcmp(token, "list")) list();
    else {
        
        printf("Unknown command: %s\r\n", token);
        
    }
    
}
