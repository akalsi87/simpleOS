/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file kmain.c
 * \date Oct 26, 2016
 */

#include "drivers/framebuf.h"

void kmain()
{
    fbinit();
    fbsetpos(2, 0);
    fbwritestr("hello!\n");
    fbwritestr("world!\n");
    for (;;) {
        // simulate halt
    }
}
