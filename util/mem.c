/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file mem.c
 * \date Oct 27, 2016
 */

#include "mem.h"

static
void copymem_forward(void* dest, void const* src, sz_t n)
{
    u8_t* d = (u8_t*)dest;
    u8_t const* s = (u8_t const*)src;
    u8_t* const d_end = dest + n;
    while (d != d_end) {
        *d++ = *s++;
    }
}

static
void copymem_reverse(void* dest, void const* src, sz_t n)
{
    u8_t* const d_end = ((u8_t*)dest)-1;
    u8_t const* s = ((u8_t const*)src)+(n-1);
    u8_t* d = d_end + n;
    while (d != d_end) {
        *d-- = *s--;
    }
}

void copymem(void* dest, void const* src, sz_t n)
{
    if (n == 0) { return; }
    void const* src_end = ((char_t*)src + n);
    return (dest >= src && dest <= src_end) ? copymem_reverse(dest, src, n)
                                            : copymem_forward(dest, src, n);
}

void fillmemb(void* dest, u8_t byte, sz_t n)
{
    u8_t* d = (u8_t*)dest;
    u8_t* const e = d + n;
    while (d != e) {
        *d++ = byte;
    }
}

void fillmemw(void* dest, u16_t word, sz_t n)
{
    u16_t* d = (u16_t*)dest;
    u16_t* const e = d + n;
    while (d != e) {
        *d++ = word;
    }
}

i32_t comparemem(void const* a, void const* b, sz_t n)
{
    i8_t* left = (i8_t*)a;
    i8_t* right = (i8_t*)b;
    i8_t* const left_end = left + n;
    i32_t res = 0;
    while (left != left_end) {
        if (*left != *right) {
            res = *left - *right;
            break;
        }
        ++left;
        ++right;
    }
    return res;
}

sz_t firstzerobyte(void const* m)
{
    char_t const* str = (char_t const*)m;
    while (*str) {
        ++str;
    }
    return ((void const*)str - m);
}
