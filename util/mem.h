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

void mem_copy(void* dest, void const* src, sz_t n);

void mem_fill_byte(void* dest, u8_t byte, sz_t n);

void mem_fill_word(void* dest, u16_t word, sz_t n);

i32_t mem_compare(void const* a, void const* b, sz_t n);

sz_t mem_first_zero_byte(void const* m);

i32_t str_compare(char_t const* a, char_t const* b);

#endif/*SIMPLEOS_UTIL_MEM_H*/
