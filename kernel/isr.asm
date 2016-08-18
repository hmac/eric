[bits 32]

; This loads the IDT descriptor into the IDT register.
; It gets called from idt.c
extern idt_desc ; The IDT descriptor
global idt_load
idt_load:
  lidt [idt_desc]

  mov ax, 0x10 ; 0x10 is the data segment offset
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:idt_finish_load ; 0x08 is the code segment offset
idt_finish_load:
  ret


; This is the generic ISR handler that will then call our C handler
; This will basically store in the stack the state of all registers,
; call the C handler, then restore the registers once it returns.
; http://wiki.osdev.org/Interrupt_Service_Routines
extern interrupt_handler
isr_redirect:
  pusha
  call interrupt_handler
  popa
  add esp, 8
  iret


; We need a separate handler for each IDT entry in order
; to distinguish between them. Each will push their interrupt
; number on to the stack, along with a dummy error code if they
; don't already come with one (so the stack is uniform between interrupts
; that have error codes and interrupts that don't.
%macro isr_err 1
 global isr %+ %1
 isr %+ %1:
   cli                       ; disable interrupts
   push byte %1              ; push interrupt code onto stack
   jmp isr_redirect
%endmacro

%macro isr_noerr 1
 global isr %+ %1
 isr %+ %1:
   cli
   push byte 0               ; push dummy error code onto stack
   push byte %1
   jmp isr_redirect
%endmacro

;; These are the handlers for ISRs 0-31
isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err   8
isr_noerr 9
isr_err   10
isr_err   11
isr_err   12
isr_err   13
isr_err   14
isr_noerr 15
isr_noerr 16
isr_noerr 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_noerr 21
isr_noerr 22
isr_noerr 23
isr_noerr 24
isr_noerr 25
isr_noerr 26
isr_noerr 27
isr_noerr 28
isr_noerr 29
isr_noerr 30
isr_noerr 31
isr_noerr 32
isr_noerr 33
isr_noerr 34
isr_noerr 35
isr_noerr 36
isr_noerr 37
isr_noerr 38
isr_noerr 39
isr_noerr 40
