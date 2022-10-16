#pragma once

#include "types.h"

#pragma aux putc = \
"mov ah, 0eh" \
"xor bx, bx" \
"int 10h" \
modify [ah bx] \
parm [al]

#pragma aux clear = \
"mov ax, 3" \
"int 10h" \
modify [ax]

#pragma aux set_attributes = \
"mov ax, 0x600" \
"mov cx, 0x0000" \
"mov dx, 0x184f" \
"int 10h" \
modify [ax cx dx] \
parm [bh]

void putc(char ch);

void puts(const char* str);
void printf(const char* fmt, ...);

void clear();
void set_attributes(uint8_t attributes); // clears the screen but keeps cursor position
