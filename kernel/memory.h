#pragma once

#include "types.h"

void memcpy(void* src, void* dest, size_t size);
void memcpy_seg(uint16_t dest_seg, uint16_t dest_off, uint16_t src_seg, uint16_t src_off, size_t size);

int8_t memcmp(void* ptr1, void* ptr2, size_t size);

void memset(void* mem, uint8_t val, size_t size);
