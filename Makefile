
ROOT ?= $(shell pwd)

.PHONY: all clean run

all: run

include boot/Makefile

clean:
	-rm -rf build

Starstone.img: $(ROOT)/build/boot/stage1.bin
	dd if=/dev/zero of=$@ bs=8192 count=180 # 8192 * 180 = 1440k
	mformat -i $@ -f 1440 ::
	dd if=$(ROOT)/build/boot/stage1.bin of=Starstone.img bs=1 count=3 conv=notrunc
	dd if=$(ROOT)/build/boot/stage1.bin of=Starstone.img bs=1 count=448 seek=62 skip=62 conv=notrunc

run: Starstone.img
	qemu-system-i386 -fda $<
