# ----- Variables ----- #

# Sources.
SOURCES = boot_kernel.asm print_string.asm load_from_disk.asm gdt.asm

# Target executable.
NAME = boot_kernel

BIN = ../bin

# ----- Make Code ----- #

all: $(NAME)

$(NAME): $(SOURCES)
	nasm boot_kernel.asm -o $(BIN)/$(NAME).bin
