GCC_OPTIONS = -g -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables -Iinclude

BUILD_DIR = ../build/

all: kernel.bin

clean:
	rm -f *.o *.bin 
	find $(BUILD_DIR) -name "*.bin" -delete

# ==== KERNEL ENTRY POINT ====

start.o: start.asm gdt_low.asm idt_low.asm irq_low.asm
	nasm -f aout -o start.o start.asm

# ==== LOW LEVEL THINGS ====

gdt.o: gdt.C include/gdt.H
	gcc $(GCC_OPTIONS) -c -o gdt.o gdt.C

idt.o: idt.C include/idt.H
	gcc $(GCC_OPTIONS) -c -o idt.o idt.C

assert.o: assert.C include/assert.H
	gcc $(GCC_OPTIONS) -c -o assert.o assert.C

utils.o: utils.C include/utils.H
	gcc $(GCC_OPTIONS) -c -o utils.o utils.C

exceptions.o: exceptions.C include/exceptions.H
	gcc $(GCC_OPTIONS) -c -o exceptions.o exceptions.C

irq.o: irq.C include/irq.H
	gcc $(GCC_OPTIONS) -c -o irq.o irq.C

interrupts.o: interrupts.C include/interrupts.H
	gcc $(GCC_OPTIONS) -c -o interrupts.o interrupts.C

page_table.o: page_table.C include/page_table.H include/paging_low.H
	gcc $(GCC_OPTIONS) -c -o page_table.o page_table.C


paging_low.o: paging_low.asm include/paging_low.H
	nasm -f aout -o paging_low.o paging_low.asm






machine.o: machine.C include/machine.H
	gcc $(GCC_OPTIONS) -c -o machine.o machine.C

machine_low.o: machine_low.asm include/machine_low.H
	nasm -f aout -o machine_low.o machine_low.asm



console.o: include/console.H console.C
	gcc $(GCC_OPTIONS) -c -o console.o console.C

timer.o: include/timer.H timer.C
	gcc $(GCC_OPTIONS) -c -o timer.o timer.C


frame_pool.o: include/frame_pool.H frame_pool.C
	gcc $(GCC_OPTIONS) -c -o frame_pool.o frame_pool.C



# ==== KERNEL MAIN FILE ====

main.o: main.C
	gcc $(GCC_OPTIONS) -c -o main.o main.C


kernel.bin: start.o utils.o main.o assert.o console.o gdt.o idt.o exceptions.o machine.o \
    machine_low.o irq.o interrupts.o link.ld timer.o frame_pool.o paging_low.o page_table.o
	ld -melf_i386 -T link.ld -o ../build/kernel.bin start.o utils.o assert.o main.o console.o gdt.o idt.o exceptions.o machine.o machine_low.o irq.o interrupts.o timer.o frame_pool.o page_table.o paging_low.o
	rm *.o
