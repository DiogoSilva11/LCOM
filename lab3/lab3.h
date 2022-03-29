#pragma once
  
#include <stdbool.h>
#include <stdint.h>
  
// Tests reading of scancodes via KBD interrupts
int kbd_test_scan();

// Tests reading of scancodes via polling
int kbd_test_poll();
  
// Tests handling of more than one interrupt
int kbd_test_timed_scan(uint8_t n);
  
// Handles keyboard interrupts (C implementation)  
void kbc_ih(void);
  
// Prints the input scancode  
// NO NEED TO IMPLEMENT  
int kbd_print_scancode(bool make, uint8_t size, uint8_t *bytes);
  
// Prints the number of sys_inb() calls
// NO NEED TO IMPLEMENT  
int kbd_print_no_sysinb(uint32_t cnt);
