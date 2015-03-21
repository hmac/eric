# ----- Variables ----- #

# Sources.
SOURCES = boot_kernel.asm print_screen.asm kernel.asm

# Target executable.
NAME = boot_kernel


# ----- Make Code ----- #

all: $(NAME)

$(NAME): $(SOURCES)
	nasm boot_kernel.asm -o $(BIN)/$(NAME).bin
