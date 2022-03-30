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

int (kbd_int_handler)(uint8_t *single_byte) {
  if (single_byte == NULL) {
    printf("null pointer\n");
    return 1;
  }

  if (util_sys_inb(KBC_OUT_BUF, single_byte)) // read only one bite per interrupt
    return 1;

  uint8_t st = 0;
  if (util_sys_inb(KBC_ST_REG, &st)) // read the status register
    return 1;

  if (st & KBC_ST_ERROR) { // check errors
    printf("error in the (serial) communication between the keyboard and the KBC");
    return 1;
  }

  return 0;
}

bool (kbd_check_break)(uint8_t *code) {
  if (code == NULL) {
    printf("null pointer\n");
    return false;
  }

  if (code == ESC_BK_CODE) 
    return true;

  return false;
}
