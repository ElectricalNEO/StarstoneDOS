#pragma once

#include "types.h"

#pragma aux putc = \
"mov ah, 0eh" \
"xor bx, bx" \
"int 10h" \
modify [ah bx] \
parm [al]

void putc(char ch);

void puts(const char* str);
