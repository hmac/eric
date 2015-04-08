#include <stdint.h>
#include "gdt.c"
#include "print.c"

typedef struct IDTEntry {
  uint16_t offset_1; // Offset bits 0..15
  uint16_t selector; // Selector - points to GDT entry for segment containing ISR
  uint8_t zero;      // 8 bits that have to be zero for some reason
  uint8_t type_attr; // Gate type (bits 0..3) and attributes (4..7)
  uint16_t offset_2; // Offset bits 16..31
} __attribute__((packed)) IDTEntry;

typedef struct IDTDescriptor {
  uint16_t limit; // Size of IDT in bytes
  uint32_t base;  // Address of start of IDT
} __attribute__((packed)) IDTDescriptor;

void add_idt_entry(IDTEntry* idt, uint8_t num, void* handler);

#define ADD_ENTRY(n) extern void isr ## n (); \
  add_idt_entry(idt, n, isr ## n)

// Create IDT Table
IDTDescriptor create_idt_table() {
    IDTEntry* idt = (IDTEntry *) GDT_START+GDT_TOTAL_SIZE;
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
    IDTDescriptor desc = {2048, (uint32_t)idt};
    return desc;
}

void add_idt_entry(IDTEntry* idt, uint8_t num, void* handler) {
  uint16_t offset_1 = (uint32_t)handler & 0xffff;             // Low 16 bits
  uint16_t offset_2 = ((uint32_t)handler & 0xffff0000) >> 16; // High 16 bits
  IDTEntry entry = { offset_1, GDT_START+8, 0, 0xe, offset_2 };
  idt[num] = entry;
}

void interrupt_handler() {
  // We will need to handle the interrupt here.
  // Exceptions should probably print an error message to screen.
  // Hardware interrupts will need to be handled individually.
  // e.g. for the keyboard, we should read the data from the PIC
  // and store it in a buffer perhaps.
  print("interrupt raised.");
}

// TODO: remap PIC IRQs (http://wiki.osdev.org/PIC#Initialisation)
// TODO: load new IDT table
// TODO: test it out
