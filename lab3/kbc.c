#include <lcom/lcf.h>

#include <stdint.h>

#include "kbc.h"
#include "i8042.h"

int (kbc_state)(uint8_t *stat) {
  if (stat == NULL) {
    printf("null pointer\n");
    return 1;
  }

  if (util_sys_inb(KBC_ST_REG, stat))
    return 1;

  return 0;
}

int (kbc_write_cmd)(uint8_t cmd) {
  uint8_t stat = 0;
  int tries = 100; // what's the ideal number? (enough time)
  while (tries) {
    if (kbc_state(&stat))
      return 1;

    /* loop while 8042 input buffer is not empty */
    if (!(stat & KBC_ST_IBF)) {
      if (sys_outb(KBC_CMD_REG, cmd)) { // no args command
        printf("no proper writing\n");
        return 1;
      }
      
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (kbc_cmd_arg)(uint8_t arg) {
  uint8_t stat = 0;
  int tries = 100; // what's the ideal number? (enough time)
  while (tries) {
    if (kbc_state(&stat))
      return 1;

    /* loop while 8042 input buffer is not empty */
    if (!(stat & KBC_ST_IBF)) {
      if (sys_outb(KBC_CMD_ARG, arg)) { // argument passed using address 0x60
        printf("no proper writing\n");
        return 1;
      }
      
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (kbc_scancode)() {
  uint8_t byte = 0;
  if (util_sys_inb(KBC_OUT_BUF, &byte)) // read only one bite (slow communication)
    return 1;

  if (kbc_inc_code()) {
    scancode[0] = KBC_FIRST_BYTE; // move 0xE0 to the first spot of the array
    scancode[1] = byte; // store byte read in the second spot
  }
  else if (byte == KBC_FIRST_BYTE)
    scancode[1] = byte; // when received, the 0xE0 byte goes to the second spot of the array
  else
    scancode[0] = byte; // one byte scancode (stored in the first spot)

  return 0;
}

int (kbc_ret_value)(uint8_t *data) {
  uint8_t stat = 0, data = 0;
  int tries = 100; // what's the ideal number? (enough time)
  while (tries) {
    if (kbc_state(&stat))
      return 1;

    /* loop while 8042 output buffer is empty */
    if ((stat & KBC_ST_OBF) && !(stat & KBC_ST_AUX)) { // if OBF is set and AUX is cleared
      if (util_sys_inb(KBC_OUT_BUF, data)) // read return value from output buffer
        return 1;

      if (!(stat & KBC_ST_ERROR)) // check error in status byte
        return 0;
      else
        return 1;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (kbc_scancode_size)() {
  if (scancode[0] == KBC_FIRST_BYTE) // scancode has two bytes if 0xE0 is in the array's first spot
    return 2;

  return 1;
}

bool (kbc_make)() {
  int byte_pos = kbc_scancode_size() - 1;
  if (scancode[byte_pos] & KBC_BREAK_BIT) // check if MSB is set to 1 (break code)
    return false;

  return true;
}

bool (kbc_inc_code)() {
  return (scancode[1] == KBC_FIRST_BYTE); // true if byte 0xE0 is in array's second spot
}

int (kbc_esc_break)() {
  /* in case the scancode is one byte */
  if (scancode[0] == ESC_BK_CODE)
    return 0;
  /* in case the scancode is two bytes */
  if (kbc_scancode_size() == 2)
    if (scancode[1] == ESC_BK_CODE)
      return 0;

  return 1;
}

static int hook_id;

int (kbc_subscribe_int)(uint8_t *bit_no) {
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

int (kbc_unsubscribe_int)() {
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

void (kbc_ih)() {
  uint8_t st = 0;
  if (kbc_state(&st)) // read the status register
    return;

  if (st & KBC_ST_ERROR) { // check errors
    printf("error in the (serial) communication between the keyboard and the KBC");
    return;
  }
  
  kbc_scancode(); // read scancode byte
}
