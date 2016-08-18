#ifndef _GDT_C_
#define _GDT_C_

#include <stdint.h>

typedef struct __attribute__((packed)) GDTEntry {
  uint16_t limit_1; // First part of the limit address
  uint16_t base_1;  // First part of the base address
  uint8_t base_2;   // Second part of the base address
  uint8_t access;   // Access byte
  uint8_t limit_2;  // Second part of limit address, plus 4 bit flags
  uint8_t base_3;  // Third part of base address
} GDTEntry;

typedef struct __attribute__((packed)) GDTDescriptor {
  uint16_t size;
  uint32_t start;
} GDTDescriptor;

#define GDT_ENTRIES 3
GDTEntry gdt[GDT_ENTRIES];
GDTDescriptor gdt_desc;
void init_gdt();
void add_gdt_entry(unsigned int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

void init_gdt() {
  add_gdt_entry(0, 0, 0, 0, 0);                // null entry 
  add_gdt_entry(1, 0, 0xffffffff, 0x9a, 0xcf); // code entry
  add_gdt_entry(2, 0, 0xffffffff, 0x92, 0xcf); // data entry
  
  gdt_desc.size = (GDT_ENTRIES*sizeof(GDTEntry)) - 1;
  gdt_desc.start = (uint32_t) &gdt[0];
 
  extern void load_gdt();
  load_gdt();
}

void add_gdt_entry(unsigned int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
  gdt[num].base_1 = (base & 0xffff);
  gdt[num].base_2 = (base >> 16) & 0xff;
  gdt[num].base_3 = (base >> 24) & 0xff;
  gdt[num].limit_1 = (limit & 0xffff);
  gdt[num].limit_2 = (limit >> 16) & 0x0f;
  gdt[num].limit_2 |= flags & 0xf0;
  gdt[num].access = access;
}


#endif
