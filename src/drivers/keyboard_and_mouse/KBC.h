#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

/**
 * @file lcom_kbc.h
 * @brief Functions for interacting with the keyboard controller (KBC).
 */

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Reads data from the specified port.
 * 
 * This function reads data from the specified port and stores it in the given data pointer.
 * 
 * @param port The port to read from.
 * @param data Pointer to where the read data will be stored.
 * @param keyboard Flag indicating whether the data is from the keyboard (1) or mouse (0).
 * @return Return 0 upon success, non-zero otherwise.
 */
int KBC_read(uint8_t port, uint8_t *data, uint8_t keyboard);

/**
 * @brief Writes a command byte to the specified port.
 * 
 * This function writes a command byte to the specified port.
 * 
 * @param port The port to write the command byte to.
 * @param command_byte The command byte to write.
 * @return Return 0 upon success, non-zero otherwise.
 */
int KBC_write(uint8_t port, uint8_t command_byte);

#endif /* _LCOM_KBC_H */
