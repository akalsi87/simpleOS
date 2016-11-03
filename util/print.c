/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file print.c
 * \date Nov 02, 2016
 */

#include "print.h"

static
void reverse_string(char_t* buf, sz_t sz)
{
    const sz_t half = sz >> 1;
    const sz_t last = sz - 1;
    for (sz_t i = 0; i < half; ++i) {
        char_t tmp = buf[i];
        buf[i] = buf[last-i];
        buf[last-i] = tmp;
    }
}

static const char_t HEX_CHARS[] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};

static
void print_hex(char_t* buf, u32_t val, u32_t maxchars)
{
    char_t* const start = buf;
    do {
        u32_t idx = val & 15;
        *buf++ = HEX_CHARS[idx];
        val = val >> 4;
    } while (val != 0);

    for (sz_t i = (buf-start); i < maxchars; ++i) {
        *buf++ = '0';
    }
    *buf++ = 'x';
    *buf++ = '0';
    maxchars = (buf-start);
    reverse_string(start, maxchars);
    start[maxchars] = '\0';
}

void buf_print_hex_32(char_t* buf, u32_t val)
{
    // -2 for 0x and -1 for '\0'
    print_hex(buf, val, HEX_PRINT_CHARS-2-1);
}

void buf_print_dec_32(char_t* buf, u32_t val)
{
    char_t* const start = buf;
    sz_t len;
    do {
        u32_t idx = val % 10;
        *buf++ = HEX_CHARS[idx];
        val = val / 10;
    } while (val != 0);
    len = (buf-start);
    reverse_string(start, len);
    start[len] = '\0';
}
