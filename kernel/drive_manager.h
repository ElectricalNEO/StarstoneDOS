#pragma once

#include "drive.h"

void init_drive_manager();

drive_t* get_drive_by_id(uint8_t id);
drive_t* get_drive_by_name(char* name); // e.g. fd0; fd1; hd0; hd1

drive_list_t* get_drives();
