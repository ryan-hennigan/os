GCC_OPTIONS = -g -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables -I./include


all: kernel.bin

clean:
	rm -f *.o *.bin

# ==== KERNEL ENTRY POINT ====

start.o: start.asm 
	nasm -f aout -o start.o start.asm


scrn.o: scrn.C
	gcc $(GCC_OPTIONS) -c -o scrn.o scrn.C

# ==== KERNEL MAIN FILE ====

main.o: main.C
	gcc $(GCC_OPTIONS) -c -o main.o main.C


kernel.bin: start.o main.o scrn.o link.ld
	ld -melf_i386 -T link.ld -o kernel.bin start.o main.o scrn.o 
