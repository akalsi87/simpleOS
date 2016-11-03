/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file assert.h
 * \date Oct 27, 2016
 */

#ifndef SIMPLEOS_ASSERT_H
#define SIMPLEOS_ASSERT_H

#include "cpu/types.h"

#define __STR_(x) #x
#define __STR(x) __STR_(x)

#define ASSERT(e) (!!(e)) ? 0 : panic("(" __FILE__ ":" __STR(__LINE__) ") " __STR(e), "")
#define ASSERT_MSG(e, m) (!!(e)) ? 0 : panic("(" __FILE__ ":" __STR(__LINE__) ") " __STR(e), m)

#define __CONCAT_(a, b) a##b
#define __CONCAT(a, b) __CONCAT_(a, b)

#define STATIC_ASSERT(e, m) \
    char __CONCAT(m, _assert)[e ? 1 : -1]

i32_t panic(char_t const* expr, char_t const* str);

#endif/*SIMPLEOS_ASSERT_H*/
