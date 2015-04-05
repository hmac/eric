load_from_disk:
	; Drive information needed by BIOS routine.
	mov ch, 0 ; Cylinder 0.
	mov dh, 0 ; Selects head 0.
	mov cl, 2 ; Selects the second sector (the one directly after boot sector).
	mov al, KERNEL_SIZE ; Reads in one sector (512 bytes).

	; Memory information, i.e. where to read the data into.
	mov bx, 0x0000
	mov es, bx
	mov bx, KERNEL_MEM_LOC

	mov ah, 0x02 ; Read sector function of BIOS read interrupt.
	int 0x13

	jc .read_error	; Jump to a error handler if carry flag is true (i.e. if error occurred).
	cmp al, KERNEL_SIZE ; Compare the number of sectors read with number expected.
	jne .read_error ; Jump to error handler if wrong number read.

	mov bx, KERNEL_LOAD_SUCCESS
	call print_string
  ret
.read_error
	mov bx, READ_ERROR_MSG
	call print_string
  ret

READ_ERROR_MSG db "Problem reading disk...", 0
KERNEL_LOAD_SUCCESS db "Kernel loaded.", 0dh, 0ah, 0
