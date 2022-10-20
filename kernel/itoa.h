#pragma once

#include "types.h"

extern char itoa_buf[];

char* itoa(uint32_t num, char* buffer, uint8_t base);
