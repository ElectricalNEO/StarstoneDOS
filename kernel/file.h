#pragma once

#include "fat12_16.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct file_t file_t;

file_t* fopen(char* path); // e.g. "/DATA/LOREMIPS.TXT"; "hd0p0/FILE.TXT"; path will be changed
void fclose(file_t* file);

int8_t fseek(file_t* file, uint32_t offset, uint8_t origin);
uint32_t ftell(file_t* file);

uint8_t fread(file_t* file, uint16_t segment, uint16_t offset, uint32_t size);

char* get_dir_entry(char* dir, size_t entry_index);

uint8_t is_dir(file_t* file);
