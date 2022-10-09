
ROOT ?= ..

$(ROOT)/build/kernel/%.bin: $(ROOT)/kernel/%.asm
	mkdir -p $(@D)
	nasm -f bin $< -o $@
