/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file framebuf.c
 * \date Oct 27, 2016
 */

#include "framebuf.h"

#include "ports.h"
#include "util/mem.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

static u16_t fb_row;
static u16_t fb_col;
static u8_t  fb_attr;

#define FB_WIDTH  80
#define FB_HEIGHT 25
#define FB_MEM    ((u16_t*)0xB8000)

void fb_get_pos(u16_t* prow, u16_t* pcol)
{
    *prow = fb_row;
    *pcol = fb_col;
}

void fb_set_pos(u16_t row, u16_t col)
{
    port_write_byte(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    port_write_byte(FB_DATA_PORT, row);
    port_write_byte(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    port_write_byte(FB_DATA_PORT, col);
    fb_row = row;
    fb_col = col;
}

void fb_cls()
{
    u16_t val = (((u16_t)0x0F) << 8) | ' ';
    mem_fill_word(FB_MEM, val, FB_WIDTH*FB_HEIGHT);
}

void fb_init()
{
    fb_row = 0;
    fb_col = 0;
    fb_attr = 0x0F;
    fb_cls();
    fb_set_pos(0, 0);
}

u8_t fb_get_attr()
{
    return fb_attr;
}

void fb_set_attr(u8_t attr)
{
    fb_attr = attr;
}

static
void fbwritechar(char_t c)
{
    if (fb_col >= FB_WIDTH) {
        fb_col = 0;
        ++fb_row;
    }
    if (fb_row >= FB_HEIGHT) {
        mem_copy(FB_MEM, FB_MEM + FB_WIDTH, (FB_HEIGHT-1)*FB_WIDTH*sizeof(u16_t));
        u16_t word = (((u16_t)0x0F) << 8) | ' ';
        mem_fill_word(FB_MEM+(FB_HEIGHT-1)*FB_WIDTH, word, FB_WIDTH);
        --fb_row;
    }
    u16_t val = (((u16_t)fb_attr) << 8) | c;
    switch (c) {
        case '\b': {
            i32_t first_in_row = (fb_col == 0);
            fb_col = first_in_row ? FB_WIDTH-1 : fb_col-1;
            fb_row = (first_in_row && fb_row != 0)? fb_row-1 : fb_row;
            FB_MEM[fb_row*FB_WIDTH + fb_col] = (((u16_t)0x0F) << 8) | ' ';
            break;
        }
        case '\n':
            fb_row++;
            fb_col = 0;
            break;
        case '\r':
            break;
        case '\t': {
            u16_t i = 0;
            val = (val & 0x00) | (u8_t)(' ');
            while ((i < 4) && (fb_col < FB_WIDTH)) {
                FB_MEM[fb_row*FB_WIDTH + fb_col++] = val;
                ++i;
            }
            break;
        }
        default:
            FB_MEM[fb_row*FB_WIDTH + fb_col++] = val;
            break;
    }
}

void fb_write_str(char_t const* str)
{
    while (*str) {
        fbwritechar(*str++);
    }
    fb_set_pos(fb_row, fb_col);
}
