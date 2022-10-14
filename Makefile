
SHELL=/bin/bash

ROOT ?= $(shell pwd)

BOOT = $(ROOT)/build/boot/boot.bin
KERNEL = $(ROOT)/build/kernel/kernel.bin

FILES_FOR_IMAGE = $(BOOT) $(KERNEL)

.PHONY: all clean run

all: run

include boot/Makefile
include kernel/Makefile

clean:
	@echo -e "\u001b[31mRM\u001b[0m build *.err mem*"
	@-rm -rf build *.err mem*

Starstone.img: $(FILES_FOR_IMAGE)
	
	@echo -e "\u001b[35mGENERATING\u001b[0m $@"
	
	@dd if=/dev/zero of=$@ bs=8192 count=180 status=none # 8192 * 180 = 1440k
	@mformat -i $@ -f 1440 ::
	
	@dd if=$(BOOT) of=$@ bs=1 count=3 conv=notrunc status=none
	@dd if=$(BOOT) of=$@ bs=1 count=448 seek=62 skip=62 conv=notrunc status=none
	
	@mcopy -i $@ $(KERNEL) ::/STARKRNL.SYS
	

run: Starstone.img
	@echo -e "\u001b[34mQEMU\u001b[0m $<"
	@qemu-system-i386 -drive file=$<,if=floppy,format=raw
