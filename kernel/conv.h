#pragma once

#include "types.h"

extern char itoa_buf[];

char* itoa(uint32_t num, char* buffer, uint8_t base);
size_t get_number_characters(char* str, uint8_t base);
uint32_t atoi(char* str, uint8_t base);
