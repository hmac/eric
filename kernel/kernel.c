#include <stdint.h>
#include <stdbool.h>
#include "print.c"
#include "gdt.c"
#include "idt.c"
#include "keyboard.c"
#include "pit.c"

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

  init_gdt();

  remap_pics();
  init_idt();

  // Set up keyboard handler
  install_interrupt_handler(1, keyboard_handler);
  
  // Set up PIT
  set_timer_freq(100);
  install_interrupt_handler(0, pit_handler);
  
  // Enable HW interrupts
  __asm__ ("sti");
  
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

