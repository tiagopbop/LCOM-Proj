#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/**
 * @defgroup i8042 i8042
 * @{
 * Constants for programming the i8042 Keyboard Controller.
 */

#include <lcom/lcf.h>

// Keyboard and Mouse IRQ Line
#define KBD_IRQ 1 /**< Keyboard IRQ line */
#define MOUSE_IRQ 12 /**< Mouse IRQ line */

// Status Register Macros
#define OBF        BIT(0) /**< Output buffer full: 1 when full (available for read), 0 when empty */
#define IBF        BIT(1) /**< Input buffer full: 1 when full (don't write), 0 when empty */
#define TIMEOUT    BIT(6) /**< Timeout error: 1 if timeout error, 0 otherwise */
#define PARITY     BIT(7) /**< Parity error: 1 if parity error, 0 otherwise */

// Scancode Macros
#define BREAK_CODE     BIT(7) /**< Break code: when (&) -> 1 if break code, 0 if make code */
#define ESC_BREAKCODE  0x81   /**< Escape break code: useful for exit condition */

// KBC Register Macros
#define KBC_STAT_REG  0x64 /**< Status Register (read KBC state) */
#define KBC_CMD_REG   0x64 /**< Input Buffer write commands to KBC */
#define KBC_OBF_REG   0x60 /**< Output Buffer to read scancodes and return values from KBC commands */
#define KBC_RCB       0x20 /**< Read Command Byte */
#define KBC_WCB       0x60 /**< Write Command Byte */

#define ENABLE_KEYBOARD_INT   BIT(0) /**< Enable keyboard interrupts */

// Mouse Macros
#define MOUSE_LB        BIT(0) /**< Left button was pressed: 1 if pressed */
#define MOUSE_RB        BIT(1) /**< Right button was pressed: 1 if pressed */
#define MOUSE_MB        BIT(2) /**< Middle button was pressed: 1 if pressed */
#define CONTROL         BIT(3) /**< Used to check if bit 3 is set to verify the potential first byte read */
#define MOUSE_X_SIGN    BIT(4) /**< X movement is negative: 1 if negative */
#define MOUSE_Y_SIGN    BIT(5) /**< Y movement is negative: 1 if negative */
#define MOUSE_X_OVF     BIT(6) /**< Overflow in the registered X movement: 1 if overflow */
#define MOUSE_Y_OVF     BIT(7) /**< Overflow in the registered Y movement: 1 if overflow */
#define EXTEND(byte)    (0xFF00 | (byte)) /**< Extend sign (1's) to 16 bits if X or Y sign are set */
#define ENABLE_DATA_REPORTING   0xF4 /**< Enable data reporting command */
#define DISABLE_DATA_REPORTING  0xF5 /**< Disable data reporting command */
#define WRITE_BYTE_TO_MOUSE     0xD4 /**< Write byte to mouse command */

// Mouse response macros
#define ACK         0xFA /**< Acknowledgement: everything is ok */
#define NACK        0xFE /**< Non-acknowledgement: invalid byte */
#define MOUSE_ERROR 0xFC /**< Mouse error: second consecutive invalid byte */

#define DELAY   20000 /**< Delay in microseconds */
#define MAX_TRIES 10 /**< Maximum number of tries */

/** @} end of i8042 */

#endif /* _LCOM_I8042_H */
