#pragma once

#include "fs.h"

void init_fs_manager();

fs_t* get_fs_by_part_id(uint8_t drive_id, uint8_t part_id);
fs_t* get_fs_by_part_name(char* name);

fs_list_t* get_all_fs();
