#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

/**
 * @file keyboard.h
 * @brief Functions for handling keyboard interrupts and communication.
 */

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "KBC.h"



/**
 * @brief Subscribes keyboard interrupts.
 *
 * @param bit_no Pointer to store the bit number to be set in the mask returned upon an interrupt.
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads the scancode from the KBC and handles any errors.
 */
void (kbc_ih)();


#endif /* _LCOM_KEYBOARD_H_ */
