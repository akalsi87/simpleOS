/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file kmain.c
 * \date Oct 26, 2016
 */

#include "drivers/framebuf.h"
#include "drivers/serial.h"

#include "util/assert.h"

void kmain()
{
    fbinit();
    serialinit();
    fbsetpos(24, 78);
    fbwritestr("hello!\n");
    fbwritestr("world!\n");

    ASSERT(1 == 0);
    ASSERT_MSG(0, "failed assert!");

    while (1) {
        for (i32_t i = 0; i < 500000; ++i) {
            // simulate halt
        }

    }
}
