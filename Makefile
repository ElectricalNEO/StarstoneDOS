
ROOT ?= $(shell pwd)

all: run

include boot/Makefile

clean:
	-rm -rf build

run: $(ROOT)/build/boot/stage1.bin
	qemu-system-i386 -fda $<
