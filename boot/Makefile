
ROOT ?= ..

$(ROOT)/build/boot/%.bin: $(ROOT)/boot/%.asm
	mkdir -p $(@D)
	nasm -f bin $< -o $@
