
SHELL=/bin/bash

ROOT ?= $(shell pwd)

STAGE1 = $(ROOT)/build/boot/stage1.bin
STAGE2 = $(ROOT)/build/boot/stage2.bin

FILES_FOR_IMAGE = $(STAGE1) $(STAGE2)

STAGE2_SIZE = $(shell stat -c %s $(STAGE2))
STAGE2_SECTORS = $$(( ($(STAGE2_SIZE) + 511) / 512 ))

RESERVED_SECTORS = $$(( $(STAGE2_SECTORS) + 1 ))

.PHONY: all clean run

all: run

include boot/Makefile

clean:
	-rm -rf build

Starstone.img: $(FILES_FOR_IMAGE)
	
	echo size: $(STAGE2_SIZE)
	echo sectors: $(STAGE2_SECTORS)
	echo rsecots: $(RESERVED_SECTORS)
	
	dd if=/dev/zero of=$@ bs=8192 count=180 # 8192 * 180 = 1440k
	mformat -i $@ -f 1440 -R $(RESERVED_SECTORS) ::
	
	dd if=$(STAGE1) of=$@ bs=1 count=3 conv=notrunc
	dd if=$(STAGE1) of=$@ bs=1 count=448 seek=62 skip=62 conv=notrunc
	
	dd if=$(STAGE2) of=$@ bs=1 seek=512 conv=notrunc
	

run: Starstone.img
	qemu-system-i386 -fda $<
