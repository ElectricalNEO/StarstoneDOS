
ROOT ?= ..

$(ROOT)/build/program/%.com: $(ROOT)/program/%.asm
	@mkdir -p $(@D)
	@echo -e "\u001b[36mNASM\u001b[0m build/program/$*.com"
	@nasm -f bin $< -o $@
