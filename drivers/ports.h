/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file ports.h
 * \date Oct 26, 2016
 */

#ifndef SIMPLEOS_DRIVERS_PORTS_H
#define SIMPLEOS_DRIVERS_PORTS_H

#include "cpu/types.h"

u8_t  port_read_byte(u16_t port);

u16_t port_read_word(u16_t port);

void  port_write_byte(u16_t port, u8_t d);

void  port_write_word(u16_t port, u16_t d);

#endif/*SIMPLEOS_DRIVERS_PORTS_H*/
