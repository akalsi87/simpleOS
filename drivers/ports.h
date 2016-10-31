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

u8_t  portreadb(u16_t port);

u16_t portreadw(u16_t port);

void  portwriteb(u16_t port, u8_t d);

void  portwritew(u16_t port, u16_t d);

#endif/*SIMPLEOS_DRIVERS_PORTS_H*/
