; This routine is placed at the top of the kernel binary.
; It is responsible for jumping execution to the actual main function,
; so we don't have to keep main at the top of kernel.c

[bits 32]
[extern main]
call main
jmp $

