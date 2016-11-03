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

static irq_t HANDLERS[256] = {0};

irq_t irq_set_handler(u8_t n, irq_t h)
{
    irq_t t = HANDLERS[n];
    HANDLERS[n] = h;
    return t;
}

void irq_handler(registers_t const* r)
{
    u32_t int_no = r->int_no;
    if (int_no >= 40) {
        // alert slave
        port_write_byte(0xA0, 0x20);
    }
    // alert master
    port_write_byte(0x20, 0x20);

    // debugwritestr("IRQ ");
    // char_t hex[HEX_PRINT_CHARS];
    // bufprinthex(hex, r.int_no);
    // debugwritestr(hex);
    // debugwritestr(" called!\n");

    {// run handler
        irq_t h = HANDLERS[int_no];
        if (h) {
            h(r);
        }
    }
}
