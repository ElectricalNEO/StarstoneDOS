#pragma once

#include "types.h"

extern char itoa_buf[];

char* itoa(size_t num, char* buffer, uint8_t base);
