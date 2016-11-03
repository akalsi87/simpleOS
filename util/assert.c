/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file assert.c
 * \date Oct 27, 2016
 */

#include "assert.h"
#include "drivers/serial.h"

STATIC_ASSERT(sizeof(char_t) == 1, incorrect_type_size_char_t);
STATIC_ASSERT(sizeof(i8_t)   == 1, incorrect_type_size_i8_t);
STATIC_ASSERT(sizeof(u8_t)   == 1, incorrect_type_size_u8_t);
STATIC_ASSERT(sizeof(i16_t)  == 2, incorrect_type_size_i16_t);
STATIC_ASSERT(sizeof(u16_t)  == 2, incorrect_type_size_u16_t);
STATIC_ASSERT(sizeof(i32_t)  == 4, incorrect_type_size_i32_t);
STATIC_ASSERT(sizeof(u32_t)  == 4, incorrect_type_size_u32_t);
STATIC_ASSERT(sizeof(i64_t)  == 8, incorrect_type_size_i64_t);
STATIC_ASSERT(sizeof(u64_t)  == 8, incorrect_type_size_u64_t);

i32_t panic(char_t const* e, char_t const* m)
{
    dbg_write_str("[PANIC] ");
    dbg_write_str(e);
    dbg_write_str(" ");
    dbg_write_str(m);
    dbg_write_str("\n");
    return 0;
}
