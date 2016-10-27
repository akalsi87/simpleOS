/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file framebuf.h
 * \date Oct 27, 2016
 */

#ifndef SIMPLEOS_DRIVERS_FRAMEBUF_H
#define SIMPLEOS_DRIVERS_FRAMEBUF_H

#include "util/types.h"

void fbinit();
void fbcls();

void fbgetpos(u16_t* prow, u16_t* pcol);
void fbsetpos(u16_t row, u16_t col);

u8_t fbgetattr();
void fbsetattr(u8_t attr);

void fbwritestr(const char_t* str);

#endif/*SIMPLEOS_DRIVERS_FRAMEBUF_H*/
