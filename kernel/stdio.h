#pragma once

#include "types.h"

#pragma aux clear = \
"mov ax, 3" \
"int 10h" \
modify [ax]

#pragma aux getc = \
"xor ah, ah" \
"int 16h" \
modify [ah] \
value [al]

void putc(char ch);

void puts(const char* str);
void printf(const char* fmt, ...);

void clear();

char getc();
void gets(char* buf, size_t buf_size);
