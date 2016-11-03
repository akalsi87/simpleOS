/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file serial.h
 * \date Oct 27, 2016
 */

#ifndef SIMPLEOS_DRIVERS_SERIAL_H
#define SIMPLEOS_DRIVERS_SERIAL_H

#include "cpu/types.h"

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

void serial_init();

void serial_cfg_port(u16_t com, u16_t divisor);

void serial_write(u16_t com, u8_t const* b, sz_t n);

#endif/*SIMPLEOS_DRIVERS_SERIAL_H*/
