
SHELL=/bin/bash

ROOT ?= $(shell pwd)

STAGE1 = $(ROOT)/build/boot/stage1.bin

FILES_FOR_IMAGE = $(STAGE1)

.PHONY: all clean run

all: run

include boot/Makefile

clean:
	-rm -rf build

Starstone.img: $(FILES_FOR_IMAGE)
	
	dd if=/dev/zero of=$@ bs=8192 count=180 # 8192 * 180 = 1440k
	mformat -i $@ -f 1440 ::
	
	dd if=$(STAGE1) of=$@ bs=1 count=3 conv=notrunc
	dd if=$(STAGE1) of=$@ bs=1 count=448 seek=62 skip=62 conv=notrunc
	

run: Starstone.img
	qemu-system-i386 -fda $<
