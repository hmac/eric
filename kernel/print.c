#include <stdint.h>

void printchar(char c);
void print(char str[]);

int term_column = 0;
int term_row = 0;
char term_colour;

#define VGA_START 0xb8000
#define TERM_WIDTH 80

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
