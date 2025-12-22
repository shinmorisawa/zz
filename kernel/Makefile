CC := clang
AS := nasm
LD := ld.lld

# sources
include src/sources.mk
include src/boot/sources.mk

CFLAGS := -Iinclude/ -ffreestanding -fno-stack-protector -fno-pic -m32 -O2 -Wall -Wextra
ASFLAGS := -f elf32
LDFLAGS := -m elf_i386 -T linker.ld -nostdlib

OBJDIR := obj
BUILDDIR := build

OBJC := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC_BOOT_C) $(SRC_C))
OBJS := $(patsubst %.s,$(OBJDIR)/%.o,$(SRC_BOOT_S))
TARGET := $(BUILDDIR)/zz

all: $(TARGET)

final: $(TARGET)
	truncate -s 262144 $(TARGET)
	cp ../boot/zzboot.bin $(BUILDDIR)
	cat ./$(BUILDDIR)/zzboot.bin $(TARGET) > $(BUILDDIR)/final.img

run: final
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR)/final.img -serial stdio

$(TARGET): $(OBJS) $(OBJC)
	mkdir -p $(BUILDDIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(OBJC)

$(OBJDIR)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)
