GCC_OPTIONS = -g -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables -I./src/include -I./src


SRC_DIR = src/

OBJ_DIR = objects/

BUILD_DIR = build/

all: kernel.bin

clean:
	rm -f *.o *.bin 
	find $(BUILD_DIR) -name "*.bin" -delete
	find $(OBJ_DIR) -name "*.o" -delete

# ==== KERNEL ENTRY POINT ====

start.o: $(addprefix $(SRC_DIR), start.asm)
	nasm -f aout -o $(addprefix $(OBJ_DIR),start.o) $(addprefix $(SRC_DIR),start.asm)


console.o: $(addprefix $(SRC_DIR),console.H console.C)
	gcc $(GCC_OPTIONS) -c -o $(addprefix $(OBJ_DIR),console.o) $(addprefix $(SRC_DIR),console.C)

# ==== KERNEL MAIN FILE ====

main.o: $(addprefix $(SRC_DIR),main.C)
	gcc $(GCC_OPTIONS) -c -o $(addprefix $(OBJ_DIR),main.o) $(addprefix $(SRC_DIR),main.C)


kernel.bin: start.o main.o console.o $(addprefix $(SRC_DIR),link.ld)
	ld -melf_i386 -T $(addprefix $(SRC_DIR),link.ld) -o $(addprefix $(BUILD_DIR),kernel.bin) $(addprefix $(OBJ_DIR),start.o main.o console.o )
