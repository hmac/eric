# ----- Variables ----- #

# Sources.
SOURCES = kernel.c

# Target executable.
NAME = kernel

BIN = ../bin


# ----- Make Code ----- #

all: $(NAME)

$(NAME): $(SOURCES)
	../xc/bin/i386-elf-gcc -ffreestanding -c kernel.c -o $(NAME).o
	../xc/bin/i386-elf-ld -o $(BIN)/kernel.bin -Ttext 0x1000 $(NAME).o --oformat binary
