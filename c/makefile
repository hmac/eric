# ----- Variables ----- #

# Sources.
SOURCES = kernel_entry.asm kernel.c

# Target executable.
NAME = kernel

BIN = ../bin


# ----- Make Code ----- #

all: $(NAME)

$(NAME): $(SOURCES)
	nasm kernel_entry.asm -f elf -o kernel_entry.o
	../xc/bin/i386-elf-gcc -Wall -Wextra -std=c11 -ffreestanding -c kernel.c -o $(NAME).o
	../xc/bin/i386-elf-ld --discard-none -o $(BIN)/kernel.bin -Ttext 0x1000 kernel_entry.o $(NAME).o --oformat binary
