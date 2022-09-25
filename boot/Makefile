
ROOT ?= ..

$(ROOT)/build/boot/stage1.bin: $(ROOT)/boot/stage1.asm
	mkdir -p $(@D)
	nasm -f bin $< -o $@
