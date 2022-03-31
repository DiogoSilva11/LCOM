#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbd.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status, r;
  message msg;

  uint8_t bit_no = 0;
  if (kbd_subscribe_int(&bit_no)) // subscribe keyboard interrupts
    return 1;
  int irq_set = BIT(bit_no); // ...01000 (bit 3)
  
  extern uint32_t cnt;
  cnt = 0;
  int process = 1;
  while (process) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
            kbc_ih();
            if (!kbd_inc_code())
              if (kbd_print_scancode(kbd_make(), kbd_scancode_size(), scancode))
                return 1;
            process = kbd_esc_break();
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (kbd_unsubscribe_int()) // unsubscribe keyboard at the end
    return 1;

  if (kbd_print_no_sysinb(cnt)) // print the number of sys_inb() kernel calls
    return 1;

  return 0;
}

int(kbd_test_poll)() {
  extern uint32_t cnt;
  cnt = 0;

  // while (...) { poll status register...

  //if (kbc_issue_cmd(KBC_RCM))

  // if (OBF = 1, AUX = 0) => read OUT_BUF

  // write uint8_t cmd (to enable interrupts) before exit

  if (kbd_print_no_sysinb(cnt)) // print the number of sys_inb() kernel calls
    return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
