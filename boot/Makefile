
ROOT ?= ..

$(ROOT)/build/boot/%.bin: $(ROOT)/boot/%.asm
	@mkdir -p $(@D)
	@echo -e "\u001b[36mNASM\u001b[0m build/boot/$*.bin"
	@nasm -f bin $< -o $@
