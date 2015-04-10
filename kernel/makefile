# ----- Variables ----- #

# Sources.
SOURCES = kernel_entry.asm kernel.c gdt.c load_gdt.asm idt.c isr.asm io.c print.c

# Target executable.
NAME = kernel

BIN = ../bin


# ----- Make Code ----- #

all: $(NAME)

$(NAME): $(SOURCES)
	nasm kernel_entry.asm -f elf -o kernel_entry.o
	nasm isr.asm -f elf -o isr.o
	nasm load_gdt.asm -f elf -o load_gdt.o
	../xc/bin/i686-elf-gcc -Wall -Wextra -std=c11 -ffreestanding -c kernel.c -o $(NAME).o
	../xc/bin/i686-elf-ld --discard-none -o $(BIN)/kernel.bin -Ttext 0x7e00 kernel_entry.o isr.o load_gdt.o $(NAME).o --oformat binary
