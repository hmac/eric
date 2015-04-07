; Procedure for printing a string to screen, expects first char address in bx.
; Real mode only
print_string:
	pusha ; Saves current register state to stack.
	mov ah, 0x0e ; Sets scrolling teletext BIOS routine.

	; Loops through characters in string, print each in turn.
	.print_loop
		cmp byte [bx], 0 ; Checks if char is 0x0 (i.e. null terminator).
		je .return ; Returns if string has ended.
		mov al, [bx] ; Prints character.
		int 0x10
		add bx, 0x1 ; Moves to next character.
		jmp .print_loop

	.return 
		popa ; Retrieves previous register state.
		ret ; Returns from procedure.
