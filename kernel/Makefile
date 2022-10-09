
ROOT ?= ..

SOURCES = $(shell find $(ROOT)/kernel -name '*.c')
OBJECTS = $(patsubst $(ROOT)/kernel/%.c, $(ROOT)/build/kernel/%.o, $(SOURCES))

$(ROOT)/build/kernel/%.o: $(ROOT)/kernel/%.c
	mkdir -p $(@D)
	gcc -c -g -Os -m32 -march=i686 -ffreestanding -Wall -Werror -masm=intel $< -o $@

$(ROOT)/build/kernel/kernel.bin: $(ROOT)/kernel/linker.ld $(OBJECTS)
	ld -static -T$(ROOT)/kernel/linker.ld -nostdlib --nmagic -o $@.elf $(OBJECTS)
	objcopy -O binary $@.elf $@
