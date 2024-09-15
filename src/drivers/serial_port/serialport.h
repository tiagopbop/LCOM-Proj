#ifndef __SERIAL_PORT_H
#define __SERIAL_PORT_H

/** @defgroup serial_port Serial Port
 * @{
 * Definitions related to serial port communication.
 */

// Base I/O port address and interrupt request number for COM1
#define COM1_UART_BASE 0x3F8 /**< COM1 UART base address */
#define COM1_IRQ 4 /**< COM1 interrupt request number */

// UART Register Offsets
#define RBR 0x00 /**< Receiver Buffer Register */
#define THR 0x00 /**< Transmitter Holding Register */
#define IER 0x01 /**< Interrupt Enable Register */
#define IIR 0x02 /**< Interrupt Identification Register */
#define FCR 0x02 /**< FIFO Control Register */
#define LCR 0x03 /**< Line Control Register */
#define MCR 0x04 /**< Modem Control Register */
#define LSR 0x05 /**< Line Status Register */
#define MSR 0x06 /**< Modem Status Register */

#define BR  0x07 /**< Battle Register (Read/Write) */

// Divisor Latch Registers (for setting baud rate)
#define DLL 0x00 /**< Divisor Latch Low Byte (Read/Write) */
#define DLM 0x01 /**< Divisor Latch High Byte (Read/Write) */

// Line Status Register (LSR) Bit Masks
#define LSR_DATA_READY (1 << 0) /**< Data ready */
#define LSR_OE (1 << 1) /**< Overrun error */
#define LSR_PE (1 << 2) /**< Parity error */
#define LSR_FE (1 << 3) /**< Framing error */
#define LSR_BI (1 << 4) /**< Break interrupt */
#define LSR_THRE (1 << 5) /**< Transmitter holding register empty */
#define LSR_TRANS_EMPTY (1 << 6) /**< Transmitter empty */
#define LSR_RCVR_FIFO_ERROR (1 << 7) /**< Receiver FIFO error */

// Interrupt Enable Register (IER) Bit Masks
#define IER_ERBFI (1 << 0) /**< Enable received data available interrupt */
#define IER_ETBEI (1 << 1) /**< Enable transmitter holding register empty interrupt */
#define IER_ELSI (1 << 2) /**< Enable receiver line status interrupt */
#define IER_EDSSI (1 << 3) /**< Enable modem status interrupt */

// Interrupt Identification Register (IIR) Bit Masks
#define IIR_NO_INT_PENDING (1 << 0) /**< No interrupt pending */
#define IIR_ID ((1 << 1) | (1 << 2) | (1 << 3)) /**< Interrupt ID bits */
#define IIR_OGN_LINE_STATUS ((1 << 1) | (1 << 2)) /**< Originated from line status */
#define IIR_OGN_RCVD_DATA_AVL (1 << 2) /**< Originated from received data available */
#define IIR_FIFO_CT ((1 << 3) | (1 << 2)) /**< FIFO control trigger */
#define IIR_OGN_TRANS_EMPTY (1 << 1) /**< Originated from transmitter holding register empty */
#define IIR_INT_PENDING 0x00 /**< Interrupt pending status */

// FIFO Control Register (FCR) Bit Masks
#define FCR_CLEAR ((1 << 0) | (1 << 1) | (1 << 2)) /**< Bits to clear the FIFO */

/** @} end of serial_port */

#endif
