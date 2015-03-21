; Some helpful facts:
; $ is the address of the current instruction
; $$ is the address of the start of the current section
; Therefore ($-$$) represents the number of bytes into the current section you are
; `times x ...` repeats ... x times
; e.g. "times 3 add eax, 1" literally produces:
;   add eax, 1
;   add eax, 1
;   add eax, 1
; `db x` causes nasm to literally emit that piece of data
; e.g. "db 5" emits 05
; dw (I think) emits a single word (i.e. 2 bytes?)
; I think there's a family of d* things
; On reflection, db might mean d(byte)

; Apparently the BIOS will load our code starting at 0x7c00,
; so we need to tell the assembler so it calculates memory offsets correctly.
; This took a long, long time to figure out.
[org 0x7c00] 


kernel_start:
  mov bx, eric
  call print_str
  jmp endless_routine

eric db "hello eric",0

endless_routine jmp $

; We expect the address of the start of the string to be in register bx
; We also expect the string to be null terminated - i.e. end in 0x00
print_str:
  pusha ; push all registers on to the stack to save their state
  mov al, [bx]
  mov ah, 0x0e
  .loop
    cmp byte [bx], 0   ; check if char is 0x0
    je .return ; return if char is 0x0
    mov byte al, [bx]
    int 0x10
    add bx, 0x1
    jmp .loop
  .return
    popa ; pop the saved registers back from the stack
    ret ; return

padding:
  times 510-($-kernel_start) db 0
  dw 0xaa55
