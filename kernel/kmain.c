/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file kmain.c
 * \date Oct 26, 2016
 */

#include "cpu/gdt.h"
#include "drivers/framebuf.h"
#include "drivers/serial.h"

#include "util/assert.h"

static
void init()
{
    gdtinit();
    fbinit();
    serialinit();
}

void kmain()
{
    init();

    fbsetpos(24, 78);
    fbwritestr("hel\blo!\n");
    fbwritestr("world!\n");

    while (1) {
        for (u32_t i = 0; i < (1 << 20); ++i) {
            // simulate delay
        }
        debugwritemsg("hi serial port...");
    }
}
