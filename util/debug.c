/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file debug.c
 * \date Nov 03, 2016
 */

#include "mem.h"
#include "drivers/serial.h"
#include "drivers/framebuf.h"

#if !defined(USE_DEBUG_FRAMEBUF)
#  define WRITE_STRING(x) serial_write(SERIAL_COM1_BASE, (u8_t const*)(x), mem_first_zero_byte((x)))
#else
#  define WRITE_STRING(x) fb_write_str((x))
#endif

void dbg_write_str(char_t const* str)
{
    WRITE_STRING(str);
}

void dbg_write_msg(char_t const* str)
{
    WRITE_STRING("[DEBUG] ");
    WRITE_STRING(str);
    WRITE_STRING("\n");
}

