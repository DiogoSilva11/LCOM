#include <lcom/lcf.h>

#include <stdint.h>

#include "kbd.h"
#include "i8042.h"

static int hook_id;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("null pointer\n");
    return 1;
  }

  hook_id = 3; // 3 for example
  *bit_no = hook_id;

  /* interrupt notification subscription */
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) { 
    printf("bad subscription\n");
    return 1;
  }

  return 0;
}

int (kbd_unsubscribe_int)() {
  /* 
  unsubscribe a previous subscription of the interrupt 
  notification associated with the specified hook_id 
  */
  if (sys_irqrmpolicy(&hook_id)) {
    printf("bad unsubscription\n");
    return 1;
  }

  return 0;
}

int (kbd_scancode)() {
  uint8_t byte = 0;
  if (util_sys_inb(KBC_OUT_BUF, &byte)) // read only one bite (slow communication)
    return 1;

  if (kbd_inc_code()) {
    scancode[0] = KBC_FIRST_BYTE; // move 0xE0 to the first spot of the array
    scancode[1] = byte; // store byte read in the second spot
  }
  else if (byte == KBC_FIRST_BYTE)
    scancode[1] = byte; // when received, the 0xE0 byte goes to the second spot of the array
  else
    scancode[0] = byte; // one byte scancode (stored in the first spot)

  return 0;
}

void (kbc_ih)() {
  uint8_t st = 0;
  if (util_sys_inb(KBC_ST_REG, &st)) // read the status register
    return;

  if (st & KBC_ST_ERROR) { // check errors
    printf("error in the (serial) communication between the keyboard and the KBC");
    return;
  }
  
  kbd_scancode(); // read scancode byte
}

int (kbd_scancode_size)() {
  if (scancode[0] == KBC_FIRST_BYTE) // scancode has two bytes if 0xE0 is in the array's first spot
    return 2;

  return 1;
}

bool (kbd_make)() {
  int byte_pos = kbd_scancode_size() - 1;
  if (scancode[byte_pos] & KBC_BREAK_BIT) // check if MSB is set to 1 (break code)
    return false;

  return true;
}

bool (kbd_inc_code)() {
  return (scancode[1] == KBC_FIRST_BYTE); // true if byte 0xE0 is in array's second spot
}

int (kbd_esc_break)() {
  /* in case the scancode is one byte */
  if (scancode[0] == ESC_BK_CODE)
    return 0;
  /* in case the scancode is two bytes */
  if (kbd_scancode_size() == 2)
    if (scancode[1] == ESC_BK_CODE)
      return 0;

  return 1;
}

int (kbc_issue_cmd)(uint8_t cmd) {
  uint8_t stat = 0;
  int tries = 100; // what's the ideal number? (enough time)
  while (tries) {
    util_sys_inb(KBC_ST_REG, &stat); // assuming it returns OK

    /* loop while 8042 input buffer is not empty */
    if (!(stat & KBC_ST_IBF)) {
      sys_outb(KBC_CMD_REG, cmd); // no args command
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (kbc_read_ret)() {
  uint8_t stat = 0, data = 0;
  int tries = 100; // what's the ideal number? (enough time)
  while (tries) {
    util_sys_inb(KBC_ST_REG, &stat); // assuming it returns OK

    /* loop while 8042 output buffer is empty */
    if (stat & KBC_ST_OBF) {
      util_sys_inb(KBC_OUT_BUF, &data); // assuming it returns OK 

      if (!(stat & KBC_ST_ERROR))
        return data;
      else
        return -1;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}
