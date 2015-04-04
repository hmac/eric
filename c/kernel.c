#include <stdint.h>
#include <stdbool.h>

void clear_screen();
void* malloc(unsigned long size);
void printchar(char c);
void print(char str[]);

#define VGA_START 0xb8000
#define TERM_WIDTH 80
#define HEAP_START 0x100000
int term_column = 0;
int term_row = 0;
char term_colour;

int main() {
  term_colour = *((char*) VGA_START+1);
  term_row = 10;

  print("Welcome to eric.");
  term_row++;
  term_column = 0;
  print("A tiny tiny kernel.");

  return 0;
}

void print(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    printchar(str[i]);
  }
}


void printchar(char c) {
  uint16_t  entry = c | term_colour << 8;
  uint16_t* vga = (uint16_t*) VGA_START;
  uint16_t index = (TERM_WIDTH*term_row) + term_column;
  vga[index] = entry;
  if (term_column == TERM_WIDTH) {
    term_column = 0;
    term_row++;
  }
  else {
    term_column++;
  }
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

