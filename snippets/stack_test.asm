mov ah, 0x0e

mov bp, 0x8000
mov sp, bp

push 'A'
push 'B'
push 'C'

pop bx
push 'D'
mov al, [0x7ffa]
int 0x10

jmp $

times 510-($-$$) db 0
dw 0xaa55