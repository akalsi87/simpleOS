/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file isr.c
 * \date Nov 03, 2016
 */

#include "isr.h"
#include "drivers/ports.h"
#include "drivers/serial.h"
#include "util/print.h"

static irq HANDLERS[256] = {0};

irq irqhandler(u8_t n, irq h)
{
    irq t = HANDLERS[n];
    HANDLERS[n] = h;
    return t;
}

void irq_handler(registers r)
{
    if (r.int_no >= 40) {
        // alert slave
        portwriteb(0xA0, 0x20);
    }
    // alert master
    portwriteb(0x20, 0x20);

    debugwritestr("IRQ ");
    char_t hex[HEX_PRINT_CHARS];
    bufprinthex(hex, r.int_no);
    debugwritestr(hex);
    debugwritestr(" called!\n");

    {// run handler
        irq h = HANDLERS[r.int_no];
        if (h) {
            h(r);
        }
    }
}
