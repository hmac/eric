; Launches the kernel.
kernel_start:
	mov bx, eric
	call print_screen
	jmp hang

; String to print to screen.
eric db "hello eric", 0

