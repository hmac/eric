#include <stdint.h>
#include <stdbool.h>
#include "print.c"
#include "gdt.c"

void clear_screen();
void* malloc(unsigned long size);

#define VGA_START 0xb8000
#define HEAP_START 0x100000

int main() {
  term_colour = *((char*) VGA_START+1);
  term_row = 10;

  print("Welcome to eric.");
  term_row++;
  term_column = 0;
  print("A tiny tiny kernel.");

  // Now we attempt to set up the GDT again.
  GDTDescriptor* gdt = create_gdt();
  load_gdt(gdt);

  return 0;
}

void clear_screen() {
  char* start = (char*) VGA_START;
  char* end = (char*) start+0xfa0;
  while (start <= end) {
    *start = 0;
    *(start+1) = term_colour;
    start = start + 2;
  }
}

// A very, very simple memory manager
void* malloc(unsigned long size) {
  static void* mem = (void*) HEAP_START;
  mem = mem + size;
  return mem - size;
}

