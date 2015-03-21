; Where to load the kernel to in memory.
%define KERNEL_MEM_LOC 0x7e00
; Number of drive sectors the kernel takes up.
%define KERNEL_SIZE 1
; Where to put the base of the stack in memory.
%define STACK_BASE 0x8b00

[org 0x7c00]

; Sets the base of the stack to be 1024 bytes above boot sector.
mov bp, STACK_BASE
mov sp, bp

; Loads the kernel from disk into memory.
load_kernel:
	mov ah, 0x02 ; Read sector function of BIOS read interrupt.

	; Drive information needed by BIOS routine.
	; mov dl, 0 ; Reads drive 1.
	mov ch, 0 ; Cylinder 0.
	mov dh, 0 ; Selects head 0.
	mov cl, 2 ; Selects the second sector (the one directly after boot sector).
	mov al, KERNEL_SIZE ; Reads in one sector (512 bytes).

	; Memory information, i.e. where to read the data into.
	mov bx, 0x0000
	mov es, bx
	mov bx, KERNEL_MEM_LOC

	; Call BIOS interrupt to read.
	int 0x13

	; Jumps to a error handler if carry flag is true (i.e. if error occurred).
	jc read_error

	; Checks correct number of sectors read.
	cmp al, KERNEL_SIZE ; Compares the number of sectors read with number expected.
	jne read_error ; Jumps to error handler if wrong number read.

	; Prints success and jumps to hang routine.
	mov bx, KERNEL_LOAD_SUCCESS
	call print_screen
	jmp kernel_start

; Handles a read error.
read_error:
	mov bx, READ_ERROR_MSG
	call print_screen
	jmp hang

READ_ERROR_MSG db "Problem reading disk...", 0
KERNEL_LOAD_SUCCESS db "Kernel loaded.", 0dh, 0ah, 0

; Hangs indefinitely.
hang jmp $

%include "print_screen.asm"

; Pads up to last two bytes with 0s, puts magic number in last two bytes.
boot_padding:
	times 510-($-$$) db 0
	dw 0xaa55

%include "kernel.asm"
