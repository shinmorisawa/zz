CC := clang
SC := nasm
TARGET := zzboot.bin

$(TARGET):
	nasm -f bin mbr.s -o mbr.bin
	nasm -f bin stage1.s -o stage1.bin
	nasm -f bin stage2.s -o stage2.bin
	
	cat mbr.bin stage1.bin stage2.bin > $(TARGET)

clean:
	rm *.bin
