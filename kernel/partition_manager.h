#pragma once

#include "mbr.h"

void init_partition_manager(char boot_drive);

partition_t* get_partition_by_id(uint8_t drive_id, uint8_t part_id);
partition_t* get_partition_by_name(char* name); // e.g. fd0p0; fd1p0; hd0p1; hd1p2; boot

partition_list_t* get_partitions();
