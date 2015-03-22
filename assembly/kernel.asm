; Launches the kernel.
kernel_start:
	mov bx, eric
	call print_screen
	jmp get_key
	jmp hang

; String to print to screen.
eric db "hello eric", 0

get_key:
	mov ah, 0
	int 0x16
	mov ah, 0x0e
	int 0x10
	jmp get_key
