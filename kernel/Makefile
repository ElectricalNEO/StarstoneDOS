
ROOT ?= ..

SOURCES = $(shell find $(ROOT)/kernel -name '*.c') $(shell find $(ROOT)/kernel -name '*.asm')
TMP = $(patsubst $(ROOT)/kernel/%.c, $(ROOT)/build/kernel/%_c.o, $(SOURCES))
OBJECTS = $(patsubst $(ROOT)/kernel/%.asm, $(ROOT)/build/kernel/%_asm.o, $(TMP))

CC = /usr/bin/watcom/binl64/wcc
LD = /usr/bin/watcom/binl64/wlink

$(ROOT)/build/kernel/%_c.o: $(ROOT)/kernel/%.c
	@mkdir -p $(@D)
	@echo -e "\u001b[32mWCC\u001b[0m build/kernel/$*_c.o"
	@$(CC) -0 -d3 -s -wx -ms -zl -zq -ecc -zp0 -wcd=111 -fo=$@  $<

$(ROOT)/build/kernel/%_asm.o: $(ROOT)/kernel/%.asm
	@mkdir -p $(@D)
	@echo -e "\u001b[36mNASM\u001b[0m build/kernel/$*_asm.o"
	@nasm -f obj $< -o $@

$(ROOT)/build/kernel/kernel.bin: $(OBJECTS)
	@echo -e "\u001b[33mWLINK\u001b[0m kernel/kernel.bin"
	@$(LD) NAME $@ FILE \{ $^ \} @$(ROOT)/kernel/linker.lnk
