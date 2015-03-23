; Launches the kernel.
kernel_start:
	mov bx, eric
	call print_screen
	jmp enter_protected_mode

; String to print to screen.
eric db "hello eric", 0

%include "gdt.asm"

; Here we attempt to switch the CPU to 32 bit protected mode
enter_protected_mode:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEGMENT:init_protected_mode

[bits 32]

; Now we should be in protected mode,
; we just need to set the registers to
; updated values.
init_protected_mode:
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call protected_mode_begin


protected_mode_message db "Successfully entered protected mode.",0

protected_mode_begin:
	mov ebx, protected_mode_message
	call clear_screen
	call pm_print_string
	jmp $

%define VGA_MEMORY 0xb8000 ; Address of video memory buffer
; Print string in protected mode
; Expects address of first char in ebx
pm_print_string:
	pusha
	mov edx, VGA_MEMORY
.loop
	mov al, [ebx]
	mov ah, 0x0f ; sets colour to be white on black
	cmp al, 0
	je .end
	mov [edx], ax ; ax is al+ah, so this copies the character+colour to VGA memory
	add ebx, 1
	add edx, 2
	jmp .loop
.end
	popa
	ret

; Clear entire VGA buffer (in protected mode?)
; This set the whole screen to black.
; Useful before printing
clear_screen:
	pusha
	mov edx, VGA_MEMORY
	mov eax, 0
.loop
	mov al, 0x00
	mov ah, 0x00
	mov [edx], ax
	add edx, 2
	cmp edx, VGA_MEMORY+0xfa0 ; that value should be the end of the VGA memory buffer
	jne .loop
.end
	popa
	ret

