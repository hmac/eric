#include <stdint.h>
#include "io.c"
#include "print.c"

#ifndef _KEYBOARD_C
#define _KEYBOARD_C

void keyboard_handler();

#define CTRL 0
#define LSHIFT 0
#define RSHIFT 0
#define ALT 0
#define SPC 0
#define CPSLK 0
#define F1 0
#define F2 0
#define F3 0
#define F4 0
#define F5 0
#define F6 0
#define F7 0
#define F8 0
#define F9 0
#define F10 0
#define F11 0
#define F12 0
#define NUM 0
#define SCRL 0
#define HOME 0
#define UP 0
#define DOWN 0
#define PGUP 0
#define PGDN 0
#define LEFT 0
#define RIGHT 0
#define END 0
#define INS 0
#define DEL 0


void keyboard_handler() {
  // For some odd reason this array isn't initialised properly outside the function..
  uint8_t keyboard_keymap[128] = {
    0, 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    CTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    LSHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', RSHIFT,
    '*', ALT, SPC, CPSLK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
    NUM, SCRL, HOME, UP, PGUP, '-', LEFT, 0, RIGHT, '+', END, DOWN, PGDN,
    INS, DEL, 0, 0, 0, F11, F12, 0
  };

  uint8_t scancode = inb(0x60); // Read from keyboard data port
  if (scancode & 0x80) {
    // Key has been released
  }
  else {
    // Key has been pressed
    printchar(keyboard_keymap[scancode]);
  }
}

#endif
