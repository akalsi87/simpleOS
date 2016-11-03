/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file kmain.c
 * \date Oct 26, 2016
 */

#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "drivers/framebuf.h"
#include "drivers/serial.h"
#include "util/print.h"
#include "util/assert.h"

static
void init()
{
    gdtinit();
    //idtinit();
    fbinit();
    serialinit();
}

void kmain()
{
    init();

    fbsetpos(24, 78);
    fbwritestr("hel\blo!\n");
    fbwritestr("world!\n");

    //__asm volatile ("int $0x3");
    //__asm volatile ("int $0x4");

    while (1) {
        //for (u32_t i = 0; i < (1 << 20); ++i) {
        for (u32_t i = 0; i < 400; ++i) {
            // simulate delay
            char_t dec[DEC_PRINT_CHARS_U32];
            char_t hex[HEX_PRINT_CHARS];
            bufprintdec(dec, i);
            bufprinthex(hex, i);
            debugwritestr("[DEBUG] ");
            debugwritestr(dec);
            debugwritestr(" => ");
            debugwritestr(hex);
            debugwritestr("\n");
        }
        debugwritemsg("hi serial port...");
    }
}
