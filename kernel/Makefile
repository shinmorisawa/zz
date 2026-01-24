CC := clang
AS := nasm
LD := ld.lld

# sources
include src/sources.mk
include src/boot/sources.mk

CFLAGS := -Iinclude/ -ffreestanding -fno-stack-protector -fno-pic -fno-pie -fno-asynchronous-unwind-tables -target x86_64-pc-none-elf -O0 -Wall -Wextra -g -mno-red-zone -include include/config.h
ASFLAGS := -f elf64
LDFLAGS := -m elf_x86_64 -T linker.ld -nostdlib

OBJDIR := obj
BUILDDIR := build

OBJC := $(patsubst %.c,$(OBJDIR)/%.c.o,$(SRC_BOOT_C) $(SRC_C))
OBJS := $(patsubst %.s,$(OBJDIR)/%.s.o,$(SRC_BOOT_S) $(SRC_S))
TARGET := $(BUILDDIR)/zz

all: $(TARGET)

final: $(TARGET)
	truncate -s 131072 $(TARGET)
	cp ../boot/zzboot.bin $(BUILDDIR)
	cat ./$(BUILDDIR)/zzboot.bin $(TARGET) > $(BUILDDIR)/final.img

run: final
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR)/final.img -serial stdio

run-debug: final
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR)/final.img -serial stdio -s -S

$(TARGET): $(OBJS) $(OBJC)
	mkdir -p $(BUILDDIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(OBJC)

$(OBJDIR)/%.s.o: src/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)
