#ifndef _LCOM_MOUSE_H
#define _LCOM_MOUSE_H

/**
 * @file mouse.h
 * @brief Functions for handling mouse interrupts and communication.
 */

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

/**
 * @brief Retrieves the mouse packet.
 *
 * @param p Pointer to the packet structure to store mouse data.
 */
void (get_mouse_packet)(struct packet *p);

/**
 * @brief Subscribes mouse interrupts.
 *
 * @param bit_no Pointer to store the bit number to be set in the mask returned upon an interrupt.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 *
 * Reads the mouse data from the KBC and handles any errors.
 */
void (mouse_ih)();

/**
 * @brief Builds the mouse packet from the bytes read.
 */
void (build_packet)();

/**
 * @brief Writes a command to the mouse.
 *
 * @param command The command byte to be written.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_write_command)(uint8_t command);

/**
 * @brief Gets the current mouse position.
 */
void current_mouse_position();

/**
 * @brief Calculates the current value given a delta.
 *
 * @param value The initial value.
 * @param delta The change in value.
 * @param min The minimum possible value.
 * @param max The maximum possible value.
 * @return The updated value within the specified range.
 */
int get_current_value(int value, int delta, int min, int max);

#endif /* _LCOM_MOUSE_H */
