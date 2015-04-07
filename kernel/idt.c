#include <stdint.h>
#include "gdt.c"

#pragma pack(push)
#pragma pack(1)
typedef struct IDTEntry {
  uint16_t offset_1; // Offset bits 0..15
  uint16_t selector; // Selector - points to GDT entry for segment containing ISR
  uint8_t zero;      // 8 bits that have to be zero for some reason
  uint8_t type_attr; // Gate type (bits 0..3) and attributes (4..7)
  uint16_t offset_2; // Offset bits 16..31
} IDTEntry;
#pragma pack(pop)

// TODO: write a function to create IDT entries

struct IDTEntry keyboard_interrupt = {
  0x001, // Not worked out yet
  GDT_START+8, // Assume the code entry is 2nd in the GDT 
  0,
  0xe,
  0x001 // Not worked out yet
};

IDTEntry* idt = (IDTEntry *) GDT_START+GDT_TOTAL_SIZE;
