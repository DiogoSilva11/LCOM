#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ) // lower boundary missing??
    return 1;

  uint8_t counting_mode_base, init_mode, counter_sel; // control word fields

  uint8_t st = 0;
  if (timer_get_conf(timer, &st)) // read configuration first
    return 1;
  uint8_t mask = 0x0F; // 4 LSB (counting mode and counting base)
  counting_mode_base = st & mask;

  init_mode = TIMER_LSB_MSB; // LSB followed by MSB

  uint8_t counter_sel;
  int port; // timer register
  switch (timer) {
    case 0:
      counter_sel = TIMER_SEL0; // [00]000000
      port = TIMER_0; // 0x40
      break;
    case 1:
      counter_sel = TIMER_SEL1; // [01]000000
      port = TIMER_1; // 0x41
      break;
    case 2:
      counter_sel = TIMER_SEL2; // [10]000000
      port = TIMER_2; // 0x42
      break;
    default:
      return 1;
  }

  uint8_t ctrl_word = counter_sel | init_mode | counting_mode_base;
  int ctrl_register = TIMER_CTRL; // 0x43
  if (sys_outb(ctrl_register, ctrl_word)) // write control word to the control register
    return 1;

  uint16_t counter_initial_val = (uint16_t) (TIMER_FREQ / freq); // freq=clock/div -> div=clock/freq

  /* load counter initial value */
  uint8_t LSB = 0, MSB = 0;
  if (util_get_LSB(counter_initial_val, &LSB))
    return 1;
  if (util_get_MSB(counter_initial_val, &MSB))
    return 1;
  /* write to the counter register */
  if (sys_outb(port, LSB))
    return 1;
  if (sys_outb(port, MSB))
    return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {   
  if (st == NULL) return 1;
  int port;
  if (timer == 0) port = TIMER_0;
  else if (timer == 1) port = TIMER_1;
  else if (timer == 2) port = TIMER_2;
  else return 1;

  int ctrl_register = TIMER_CTRL;
  u32_t ctrl_word = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  if (sys_outb(ctrl_register, ctrl_word)) return 1;

  if (util_sys_inb(port, st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
   union timer_status_field_val conf;
  uint8_t mask;
  switch (field) {
    case tsf_all: // status
      conf.byte = st; // status byte
      break;
    case tsf_initial: // initialization mode
      mask = 0x30; // 00110000 (bits 4 and 5)
      uint8_t init = (st & mask) >> 4; // to isolate value
      switch (init) {
        case 0:
          conf.in_mode = INVAL_val;
          break;
        case 1:
          conf.in_mode = LSB_only; // 01
          break;
        case 2:
          conf.in_mode = MSB_only; // 10
          break;
        case 3:
          conf.in_mode = MSB_after_LSB; // 11
          break;
        default:
          return 1;
      }
      break;
    case tsf_mode: // counting mode
      mask = 0x0E; // 00001110 (bits 1, 2 and 3)
      conf.count_mode = (st & mask) >> 1; // to isolate value
      if (conf.count_mode == 6 || conf.count_mode == 7) { // 110 or 111 (compatibility values)
        mask = 0x03; // ...011
        conf.count_mode &= mask; // will result in 010 (mode 2) or 011 (mode 3)
      }
      break;
    case tsf_base: // counting base
      mask = 0x01; // 00000001 (bit 0)
      conf.bcd = st & mask; // to isolate value (1 = true)
      break;
  default:
    return 1;
  }

  if (timer_print_config(timer, field, conf)){
     return 1;
  } // to check 
  return 0;
}
