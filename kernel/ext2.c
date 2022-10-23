#include "ext2.h"
#include "fs.h"

uint8_t init_ext2(partition_t* partition, fs_t* fs) {
    
    fs->partition = *partition;
    fs->fs.ext2.test = 0x15;
    return 0;
    
}
