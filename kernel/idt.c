#ifndef _IDT_C_
#define _IDT_C_

#include <stdint.h>
#include "print.c"
#include "io.c"

void init_idt();
void create_idt_table();
void clear_idt();
void add_idt_entry(uint8_t num, uint32_t handler);
void load_idt();
void interrupt_handler();
void remap_pics();
void sendEOI(uint8_t irq);
void install_interrupt_handler(int irq, void (*handler)());

typedef struct IDTEntry {
  uint16_t offset_1; // Offset bits 0..15
  uint16_t selector; // Selector - points to GDT entry for segment containing ISR
  uint8_t zero;      // 8 bits that have to be zero for some reason
  uint8_t flags; // Gate type (bits 0..3) and attributes (4..7)
  uint16_t offset_2; // Offset bits 16..31
} __attribute__((packed)) IDTEntry;

typedef struct IDTDescriptor {
  uint16_t limit; // Size of IDT in bytes
  uint32_t base;  // Address of start of IDT
} __attribute__((packed)) IDTDescriptor;

void init_idt() {
  clear_idt();
  create_idt_table();
  load_idt();
}

#define ADD_ENTRY(n) extern void isr ## n (); \
  add_idt_entry(n, (uint32_t) isr ## n)

IDTEntry idt[256];
IDTDescriptor idt_desc;
void (*InterruptHandlers[16])();
void create_idt_table() {
    ADD_ENTRY(0);  // Unfortunately I don't think there's a more concise way of writing this.
    ADD_ENTRY(1);
    ADD_ENTRY(2);
    ADD_ENTRY(3);
    ADD_ENTRY(4);
    ADD_ENTRY(5);
    ADD_ENTRY(6);
    ADD_ENTRY(7);
    ADD_ENTRY(8);
    ADD_ENTRY(9);
    ADD_ENTRY(10);
    ADD_ENTRY(11);
    ADD_ENTRY(12);
    ADD_ENTRY(13);
    ADD_ENTRY(14);
    ADD_ENTRY(15);
    ADD_ENTRY(16);
    ADD_ENTRY(17);
    ADD_ENTRY(18);
    ADD_ENTRY(19);
    ADD_ENTRY(20);
    ADD_ENTRY(21);
    ADD_ENTRY(22);
    ADD_ENTRY(23);
    ADD_ENTRY(24);
    ADD_ENTRY(25);
    ADD_ENTRY(26);
    ADD_ENTRY(27);
    ADD_ENTRY(28);
    ADD_ENTRY(29);
    ADD_ENTRY(30);
    ADD_ENTRY(31);
    ADD_ENTRY(32);
    ADD_ENTRY(33);
    ADD_ENTRY(34);
    ADD_ENTRY(35);
    ADD_ENTRY(36);
    ADD_ENTRY(37);
    ADD_ENTRY(38);
    ADD_ENTRY(39);
    ADD_ENTRY(40);

    idt_desc.limit = (sizeof(IDTDescriptor) *256);
    idt_desc.base = (uint32_t) &idt[0];
}

void clear_idt() {
  IDTEntry zero = {0, 0, 0, 0, 0};
  for(int i = 0; i < 3; i++) {
    idt[i] = zero;
  }
}

void add_idt_entry(uint8_t num, uint32_t handler) {
  idt[num].offset_1 = (handler & 0xffff);
  idt[num].offset_2 = (handler >> 16) & 0xffff;
  idt[num].selector = 0x08;
  idt[num].zero = 0;
  idt[num].flags = 0x8e;
}

extern void idt_load();
void load_idt() {
  idt_load();
}

typedef struct ISRRegisters {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
} ISRRegisters;

void interrupt_handler(ISRRegisters regs){
  
  // Exceptions should probably print an error message to screen.
  // Int numbers < 32 are exceptions
  // 32 is the timer
  // 33 is the keyboard

  // Check if there is a handler registered for this interrupt
  void (*h)();
  if ((h = InterruptHandlers[regs.int_no-32])) {
    h();
  }
  sendEOI(regs.int_no);
}

void install_interrupt_handler(int irq, void (*handler)()) {
  InterruptHandlers[irq] = handler;
}

/*
 * The default config for IRQ -> interrupt number is:
 * IRQ     INT (dec)  INT (hex)
 * 0-7     8-15       0x08-0x0f
 * 8-15    112-119    0x70-0x77
 * 
 * However Intel CPUs use INT 0-31 for exceptions, so we need to remap IRQs 0-7.
 * In order to change anything with the PICs we have to completely re-initialise them,
 * so this is a quite a complex process.
 * First we send both PICs an initialise command (ICW1). They will wait for 3 additional "command words"
 * to complete the initialisation.
 * ICW1 - some configuration settings
 * ICW2 - offset: the address in memory that the PIC should be mapped to (i.e. the range offset-offset+7)
 * ICW3 - tell the PIC whether it's a master or a slave
 * ICW4 - some more environment configuration
 *
 * (http://wiki.osdev.org/PIC#Initialisation)
 */
#define PIC1 0x20 /* IO address for PIC 1*/ 
#define PIC2 0xa0 /* IO address for PIC 2*/ 
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1+1) /* the data port is next to the command port */ 
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2+1)
#define PIC_EOI 0x20 /* PIC End-Of-Interrupt signal - interrupt handlers must end by sending this */ 

#define ICW1      0x10  /* the base ICW1 value to start the init process */ 
#define ICW1_ICW4 0x01  /* additional bit to indicate that ICW4 will be given */ 
#define ICW4      0x01  /* this tells the PICs they are in 8086 mode (no idea) */ 
#define ICW3_PIC1 4     /* this tells PIC 1 it is the master, and the slave is at IRQ2 */
#define ICW3_PIC2 2     /* this tells PIC 2 is it the slave, and sets its cascade identity (?) - don't ask */

#define PIC1_OFFSET 0x20 /* the new offsets we want to use */ 
#define PIC2_OFFSET 0x28
void remap_pics() {
  uint8_t mask1 = inb(PIC1_DATA); /* save the PIC masks */ 
  uint8_t mask2 = inb(PIC2_DATA);

  outb(PIC1_CMD, ICW1+ICW1_ICW4); /* start the init process */ 
  io_wait();
  outb(PIC2_CMD, ICW1+ICW1_ICW4);
  io_wait();

  outb(PIC1_DATA, PIC1_OFFSET);   /* set the new offsets */ 
  io_wait();
  outb(PIC2_DATA, PIC2_OFFSET);
  io_wait();

  outb(PIC1_DATA, ICW3_PIC1);     /* set the master-slave config */ 
  io_wait();
  outb(PIC2_DATA, ICW3_PIC2);
  io_wait();

  outb(PIC1_DATA, ICW4);          /* final config */ 
  io_wait();
  outb(PIC2_DATA, ICW4);
  io_wait();

  /*outb(PIC1_DATA, mask1);         [> restore the masks <] */
  /*outb(PIC2_DATA, mask2);*/
  outb(PIC1_DATA, 0x0);
  outb(PIC2_DATA, 0x0);
}

void sendEOI(uint8_t irq) {
  if (irq >= 8) {
    outb(PIC2_CMD, PIC_EOI);
  }
  outb(PIC1_CMD, PIC_EOI);
}

#endif
