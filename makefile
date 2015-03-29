# ----- Variables ----- #

# Compiler.
export CC = gcc

# Root dir.
ROOT = $(CURDIR)
# Source code location.
ASSEMBLY = $(ROOT)/assembly
C = $(ROOT)/c
# Executable location.
export BIN = $(ROOT)/bin


# ----- Make Code ----- #

all:
	$(MAKE) -C $(ASSEMBLY)
	$(MAKE) -C $(C)
	cat $(BIN)/boot_kernel.bin $(BIN)/kernel.bin > $(BIN)/os-image

run:
	qemu-system-i386 $(BIN)/os-image

clean:
	rm $(BIN)/*
	rm $(C)/*.o
