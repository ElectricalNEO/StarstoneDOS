#pragma once

#include "fat12_16.h"

void init_fs_manager();

fat12_16_t* get_fs_by_part_id(uint8_t drive_id, uint8_t part_id);
fat12_16_t* get_fs_by_part_name(char* name);

fat12_16_list_t* get_all_fs();
