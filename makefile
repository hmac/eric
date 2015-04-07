# ----- Variables ----- #

# Compiler.
export CC = gcc

# Root dir.
ROOT = $(CURDIR)
# Source code location.
BOOT = $(ROOT)/boot
KERNEL = $(ROOT)/kernel
# Executable location.
export BIN = $(ROOT)/bin


# ----- Make Code ----- #

all: clean
	mkdir -p bin
	$(MAKE) -C $(BOOT)
	$(MAKE) -C $(KERNEL)
	cat $(BIN)/boot_kernel.bin $(BIN)/kernel.bin > $(BIN)/os-image

.PHONY: run
run: all
	qemu-system-i386 $(BIN)/os-image

.PHONY: clean
clean:
	-rm $(BIN)/*
	-rm $(KERNEL)/*.o
