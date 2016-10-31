/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file mem.h
 * \date Oct 27, 2016
 */

#ifndef SIMPLEOS_UTIL_MEM_H
#define SIMPLEOS_UTIL_MEM_H

#include "cpu/types.h"

void copymem(void* dest, void const* src, sz_t n);

void fillmemb(void* dest, u8_t byte, sz_t n);

void fillmemw(void* dest, u16_t word, sz_t n);

i32_t comparemem(void const* a, void const* b, sz_t n);

sz_t firstzerobyte(void const* m);

#endif/*SIMPLEOS_UTIL_MEM_H*/
