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

all: clean
	$(MAKE) -C $(ASSEMBLY)
	$(MAKE) -C $(C)
	cat $(BIN)/boot_kernel.bin $(BIN)/kernel.bin > $(BIN)/os-image

.PHONY: run
run: all
	qemu-system-i386 $(BIN)/os-image

.PHONY: clean
clean:
	-rm $(BIN)/*
	-rm $(C)/*.o
