CC := clang
SC := nasm
LD := ld.lld
TARGET := zzboot.bin

$(TARGET):
	$(SC) -f elf64 mbr.s -o mbr.o
	$(SC) -f elf64 stage1.s -o stage1.o
	$(SC) -f elf64 stage2.s -o stage2.o
	$(CC) -c -ffreestanding -fno-stack-protector -fno-pic -fno-pie -fno-asynchronous-unwind-tables -target x86_64-pc-none-elf -O0 -Wall -Wextra -g -mno-red-zone loader.c -o loader.o

	$(LD) -T linker.ld -o zzboot.bin mbr.o stage1.o stage2.o loader.o

clean:
	rm *.bin
	rm *.o
