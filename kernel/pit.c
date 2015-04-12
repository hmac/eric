#ifndef _PI_C_
#define _PI_C_
#include <stdint.h>
#include "io.c"
#include "idt.c"
#include "print.c"

// The PIT will set its frequency to 
// 1193180 divided by the number you give it
// Command port: 0x43
// Data port: 0x40
// http://www.osdever.net/bkerndev/Docs/pit.htm
void set_timer_freq(freq) {
  uint16_t div = 1193180/freq;
  outb(0x43, 0x36); // 0x36 is a combination several bit flags
  outb(0x40, div & 0xff); // we have to send the MSB and LSB separately
  outb(0x40, div >> 8);
}

int ticks = 0;

void pit_handler() {
  ticks++;

}

#endif
