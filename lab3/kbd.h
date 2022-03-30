#ifndef __KBD_H
#define __KBD_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup kbd kbd
 * @{
 *
 * Functions for using the i8042 keyboard controller
 */

/**
 * @brief Subscribes and enables KBC interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes KBC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_unsubscribe_int)();

/**
 * @brief KBC interrupt handler
 * 
 * @param single_byte one byte which is read upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_int_handler)(uint8_t *single_byte);

/**
 * @brief Checks if a scancode corresponds to the ESC break code
 * 
 * @param code code read upon an interrupt
 * @return Return true upon success and false otherwise
 */
bool(kbd_check_break)(uint8_t *code);

#endif /* __KBD_H */