#include "syscall.h"

extern void int21();
extern void int23();

void init_syscalls() {
    
    uint32_t* ivt = (uint32_t*)0;
    ivt[0x21] = (uint32_t)int21;
    ivt[0x23] = (uint32_t)int23;
    
}
