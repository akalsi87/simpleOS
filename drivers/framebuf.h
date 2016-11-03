/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file framebuf.h
 * \date Oct 27, 2016
 */

#ifndef SIMPLEOS_DRIVERS_FRAMEBUF_H
#define SIMPLEOS_DRIVERS_FRAMEBUF_H

#include "cpu/types.h"

void fb_init();
void fb_cls();

void fb_get_pos(u16_t* prow, u16_t* pcol);
void fb_set_pos(u16_t row, u16_t col);

u8_t fb_get_attr();
void fb_set_attr(u8_t attr);

void fb_write_str(char_t const* str);

#endif/*SIMPLEOS_DRIVERS_FRAMEBUF_H*/
