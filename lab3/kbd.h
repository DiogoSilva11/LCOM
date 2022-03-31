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

uint8_t scancode[2]; // array with the scancode bytes

/**
 * @brief Reads scancode byte from the KBC's output buffer
 * 
 * @return Return 0 upon success and false otherwise
 */
int(kbd_scancode)();

/** 
 * @brief Interrupt handler to read scancodes in C.
 */
void(kbc_ih)();

/**
 * @brief Gets the size of the scancode (assuming there is one)
 * 
 * @return Return 2 if it's a two byte scancode and 1 otherwise
 */
int(kbd_scancode_size)();

/**
 * @brief Checks if the scancode is a make or a break code
 * 
 * @return Return true if it's make and false otherwise
 */
bool(kbd_make)();

/**
 * @brief Checks if a two byte scancode is incomplete (second byte yet to store)
 * 
 * @return Return true if incomplete and false otherwise
 */
bool(kbd_inc_code)();

/**
 * @brief Checks if the scancode corresponds to the ESC break code
 * 
 * @return Return 0 upon success and false otherwise
 */
int(kbd_esc_break)();

/**
 * @brief Issuing a command to the KBC
 *
 * @param cmd command byte
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_issue_cmd)(uint8_t cmd);

#endif /* __KBD_H */