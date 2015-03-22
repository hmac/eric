# ----- Variables ----- #

# Compiler.
export CC = gcc

# Root dir.
ROOT = $(CURDIR)
# Source code location.
ASSEMBLY = $(ROOT)/assembly
# Executable location.
export BIN = $(ROOT)/bin


# ----- Make Code ----- #

all:
	mkdir -p bin
	$(MAKE) -C $(ASSEMBLY)

run:
	qemu-system-i386 bin/boot_kernel.bin

clean:
	rm bin/*
