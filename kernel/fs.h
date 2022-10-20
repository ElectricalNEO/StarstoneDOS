#pragma once

#include "mbr.h"

typedef enum filesystem_type {
    
    FAT12,
    FAT16,
    UNKNOWN
    
} filesystem_type_t;

filesystem_type_t detect_filesystem(partition_t* partition);
