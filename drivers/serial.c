/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file serial.c
 * \date Oct 27, 2016
 */

#include "serial.h"
#include "ports.h"
#include "util/mem.h"

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

static
void cfg_baud_rate(u16_t com, u16_t divisor)
{
    port_write_byte(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    port_write_byte(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    port_write_byte(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

static
void cfg_line(u16_t com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    port_write_byte(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

static
void cfg_buf(u16_t com)
{
    /*
     * Buffer config register layout:
     *
     *	| 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
     *	| lvl | bs | r | dma | clt | clr | e |
     *
     * Where:
     *	+ lvl => Size of the buffer in bytes
     *	+ bs => Enables 64 byte FIFO
     *	+ r => Reserved for future use
     *	+ dma => DMA mode selection
     *	+ clt => Clear the transmission FIFO
     *	+ clr => Clear the reception FIFO
     *	+ e => Enables or disables the FIFO buffer
     *
     * The default value by convention, for now, will be
     * one which enables the FIFO, clears both buffers and
     * uses 14 bytes of size, i.e. 0xC7.
     */
    port_write_byte(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

static
void cfg_modem(u16_t com)
{
    /*
     * Modem control register:
     *
     *	| 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
     *	| r | r | af | lb | ao2 | ao1 | rts | dtr |
     *
     * Where:
     *	+ r => Reserved
     *	+ af => Enables autoflow control (not used)
     *	+ lb => Enables loopback mode. In loopback mode
     *		the controller disconnects the receiver
     *		serial input and redirects it to the
     *		transmitter. Used for debugging
     *	+ ao2 => Auxiliary output 2, used for receiving
     *		interrupts
     *	+ ao1 => Auxiliary output 1
     *	+ rts => Ready to transmit (RTS) bit
     *	+ dtr => Data terminal ready (DTR) bit
     *
     * The default value to use will be 0x03
     */
    port_write_byte(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

void serial_cfg_port(u16_t com, u16_t div)
{
    cfg_baud_rate(com, div);
    cfg_line(com);
    cfg_buf(com);
    cfg_modem(com);
}

static
u8_t is_tx_fifo_empty(u16_t com)
{
    /* bit 5 of line status register indicates if queue is empty */
    return port_read_byte(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write(u16_t com, u8_t const* b, sz_t n)
{
    u8_t const* e = b + n;
    while (b != e) {
        while (!is_tx_fifo_empty(com)) {
            // wait for data to be writable
        }
        port_write_byte(com, *b++);
    }
}

void serial_init()
{
    serial_cfg_port(SERIAL_COM1_BASE, 1);
}
