/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file framebuf.c
 * \date Oct 27, 2016
 */

#include "framebuf.h"

#include "ports.h"

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

void fbgetpos(u16_t* prow, u16_t* pcol)
{
    *prow = fb_row;
    *pcol = fb_col;
}

void fbsetpos(u16_t row, u16_t col)
{
    portwriteb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    portwriteb(FB_DATA_PORT, row);
    portwriteb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    portwriteb(FB_DATA_PORT, col);
    fb_row = row;
    fb_col = col;
}

void fbcls()
{
    u16_t val = (((u16_t)fb_attr) << 8) | ' ';
    for (u32_t i = 0; i < FB_HEIGHT*FB_WIDTH; ++i) {
        FB_MEM[i] = val;
    }
}

void fbinit()
{
    fb_row = 0;
    fb_col = 0;
    fb_attr = 0x0F;
    fbcls();
    fbsetpos(0, 0);
}

u8_t fbgetattr()
{
    return fb_attr;
}

void fbsetattr(u8_t attr)
{
    fb_attr = attr;
}

static void fbwritechar(char_t c)
{
    u16_t val = (((u16_t)fb_attr) << 8) | c;
    switch (c) {
        case '\n':
            fb_row++;
            fb_col = 0;
            break;
        case '\r':
            break;
        case '\t': {
            u16_t i = 0;
            val = (val & 0x00) | (u8_t)(' ');
            while (i < 4 && fb_col < FB_WIDTH) {
                FB_MEM[fb_row*FB_WIDTH + fb_col++] = val;
            }
            break;
        }
        default:
            FB_MEM[fb_row*FB_WIDTH + fb_col++] = val;
            break;
    }
    if (fb_col >= FB_WIDTH) {
        fb_col = 0;
        ++fb_row;
    }
    if (fb_row >= FB_HEIGHT) {
        --fb_row;
        /* copy chars */
    }
}

void fbwritestr(const char_t* str)
{
    while (*str) {
        fbwritechar(*str++);
    }
    fbsetpos(fb_row, fb_col);
}
