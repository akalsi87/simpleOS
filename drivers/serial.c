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
void cfgbaudrate(u16_t com, u16_t divisor)
{
    portwriteb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    portwriteb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    portwriteb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

static
void cfgline(u16_t com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    portwriteb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

static
void cfgbuf(u16_t com)
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
    portwriteb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

static
void cfgmodem(u16_t com)
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
    portwriteb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

void serialcfgport(u16_t com, u16_t div)
{
    cfgbaudrate(com, div);
    cfgline(com);
    cfgbuf(com);
    cfgmodem(com);
}

static
u8_t istxfifoempty(u16_t com)
{
    /* bit 5 of line status register indicates if queue is empty */
    return portreadb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serialwrite(u16_t com, u8_t const* b, sz_t n)
{
    u8_t const* e = b + n;
    while (b != e) {
        while (!istxfifoempty(com)) {
            // wait for data to be writable
        }
        portwriteb(com, *b++);
    }
}

void serialinit()
{
    serialcfgport(SERIAL_COM1_BASE, 1);
}

void debugwritestr(char_t const* str)
{
    serialwrite(SERIAL_COM1_BASE, (u8_t const*)str, firstzerobyte(str));
}

void debugwritemsg(char_t const* str)
{
    serialwrite(SERIAL_COM1_BASE, (u8_t const*)"[DEBUG] ", 8);
    serialwrite(SERIAL_COM1_BASE, (u8_t const*)str, firstzerobyte(str));
    serialwrite(SERIAL_COM1_BASE, (u8_t const*)"\n", 1);
}
