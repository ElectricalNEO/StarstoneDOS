#include "syscall.h"

extern void syscall();

void init_syscalls() {
    
    uint32_t* ivt = (uint32_t*)0;
    ivt[0x21] = (uint32_t)syscall;
    
}
