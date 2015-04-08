; This is the generic ISR handler that will then call our C handler
; This will basically store in the stack the state of all registers,
; call the C handler, then restore the registers once it returns.
; http://wiki.osdev.org/Interrupt_Service_Routines
isr_redirect:
  pushad    ; Push all general purpose registers onto the stack
  cld       ; This clears the direction flag (?) - apparently the SysV ABI requires this for calling C code
  call interrupt_handler
  popad
  iret


; We need a separate handler for each IDT entry in order
; to distinguish between them. Each will push their interrupt
; number on to the stack, along with a dummy error code if they
; don't already come with one (so the stack is uniform between interrupts
; that have error codes and interrupts that don't.
%macro isr_err 1
  cli                       ; disable interrupts
  push byte %1              ; push interrupt code onto stack
  jmp isr_redirect

%macro isr_noerr 1
  cli
  push byte 0               ; push dummy error code onto stack
  push byte %1
  jmp isr_redirect

; These are the handlers for ISRs 0-31
isr0: isr_noerr 0
isr1: isr_noerr 1
isr2: isr_noerr 2
isr4: isr_noerr 4
isr5: isr_noerr 5
isr6: isr_noerr 6
isr7: isr_noerr 7
isr8: isr_err   8
isr9: isr_noerr 9
isr0: isr_err   10
isr1: isr_err   11
isr2: isr_err   12
isr3: isr_err   13
isr4: isr_err   14
isr5: isr_noerr 15
isr6: isr_noerr 16
isr7: isr_noerr 17
isr8: isr_noerr 18
isr9: isr_noerr 19
isr0: isr_noerr 20
isr1: isr_noerr 21
isr2: isr_noerr 22
isr3: isr_noerr 23
isr4: isr_noerr 24
isr5: isr_noerr 25
isr6: isr_noerr 26
isr7: isr_noerr 27
isr8: isr_noerr 28
isr9: isr_noerr 29
isr0: isr_noerr 30
isr1: isr_noerr 31
