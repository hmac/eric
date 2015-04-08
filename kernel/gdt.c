#include <stdint.h>

typedef struct GDTEntry {
  uint16_t limit_1; // First part of the limit address
  uint16_t base_1;  // First part of the base address
  uint8_t base_2;   // Second part of the base address
  uint8_t access;   // Access byte
  uint8_t limit_2;  // Second part of limit address, plus 4 bit flags
  uint16_t base_3;  // Third part of base address
} __attribute__((packed)) GDTEntry;

typedef struct GDTDescriptor {
  uint16_t size;
  uint32_t start;
} __attribute__((packed)) GDTDescriptor;

#define GDT_START 0xa510
#define GDT_ENTRIES 3
#define GDT_SIZE GDT_ENTRIES*8
#define GDT_TOTAL_SIZE GDT_SIZE+6

void load_gdt(GDTDescriptor *gdt);
GDTDescriptor* create_gdt();

GDTDescriptor* create_gdt() {
  GDTEntry null_entry = { 0, 0, 0, 0, 0, 0 };
  GDTEntry code_entry = {
    0xffff,
    0,
    0,
    0b10011010,
    0b11001111,
    0
  };
  GDTEntry data_entry = {
    0xffff,
    0,
    0,
    0b10010010,
    0b11001111,
    0
  };
  GDTDescriptor desc = { GDT_SIZE-1, GDT_START };
  GDTEntry* gdt_mem = (GDTEntry *) GDT_START;
  gdt_mem[0] = null_entry;
  gdt_mem[1] = code_entry;
  gdt_mem[2] = data_entry;
  // TODO: Check if this can be replaced with &gdt_mem[3]
  GDTDescriptor* desc_mem = (GDTDescriptor *) &gdt_mem[2] + sizeof(data_entry);
  *desc_mem = desc;
  return desc_mem;
}

void load_gdt(GDTDescriptor *gdt) {
  __asm__(".intel_syntax noprefix\n\t"
          "lgdt [%0]\n\t"
          ".att_syntax prefix"
          :
          : "p"(gdt)
          );
}
