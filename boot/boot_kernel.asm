; Where to load the kernel to in memory.
%define KERNEL_MEM_LOC 0x7e00
; Number of drive sectors the kernel takes up.
%define KERNEL_SIZE 2
; Where to put the base of the stack in memory.
%define STACK_BASE 0x8b00

[org 0x7c00]

; Sets the base of the stack to be 1024 bytes above boot sector.
mov bp, STACK_BASE
mov sp, bp
call load_kernel
call enter_protected_mode
jmp $

; The order we're going to do things is:
; 1. Load 2nd sector (the C kernel)
; 2. Emit the GDT
; 3. Switch to protected mode
; 4. Jump to the C kernel

%include "print_string.asm"
%include "load_from_disk.asm"
%include "gdt.asm"

LOADING_KERNEL_MESSAGE db "Loading kernel into memory...", 0

load_kernel:
	mov bx, LOADING_KERNEL_MESSAGE
	call print_string

	mov bx, KERNEL_MEM_LOC
	mov cl, 2 ; start at 2nd sector
	mov al, KERNEL_SIZE ; read 1 sector
	call load_from_disk
	ret

; Here we attempt to switch the CPU to 32 bit protected mode
enter_protected_mode:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEGMENT:init_protected_mode

[bits 32]

; Update registers for protected mode
init_protected_mode:
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call KERNEL_MEM_LOC ; Jump to C code
	jmp $

; Pads up to last two bytes with 0s, puts magic number in last two bytes.
boot_padding:
	times 510-($-$$) db 0
	dw 0xaa55

