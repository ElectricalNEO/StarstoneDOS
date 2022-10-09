
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
	-rm -rf build *.err

Starstone.img: $(FILES_FOR_IMAGE)
	
	dd if=/dev/zero of=$@ bs=8192 count=180 # 8192 * 180 = 1440k
	mformat -i $@ -f 1440 ::
	
	dd if=$(BOOT) of=$@ bs=1 count=3 conv=notrunc
	dd if=$(BOOT) of=$@ bs=1 count=448 seek=62 skip=62 conv=notrunc
	
	mcopy -i $@ $(KERNEL) ::/STARKRNL.SYS
	

run: Starstone.img
	qemu-system-i386 -fda $<
