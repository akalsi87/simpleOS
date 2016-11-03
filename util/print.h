/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file print.h
 * \date Nov 02, 2016
 */

#ifndef SIMPLEOS_UTIL_PRINT_H
#define SIMPLEOS_UTIL_PRINT_H

#include "cpu/types.h"

#define HEX_PRINT_CHARS 11
void bufprinthex(char_t* buf, u32_t val);

#define DEC_PRINT_CHARS_U32 11
void bufprintdec(char_t* buf, u32_t val);

#endif/*SIMPLEOS_UTIL_PRINT_H*/
