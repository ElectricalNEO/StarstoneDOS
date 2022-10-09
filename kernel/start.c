__asm__(".code16gcc\n");
__asm__("jmp main\n");

void main() {
    
    __asm__ volatile("mov ax, 0x5678");
    while(1);
    
}
