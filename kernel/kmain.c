/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file kmain.c
 * \date Oct 26, 2016
 */

#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/isr.h"
#include "cpu/timer.h"
#include "drivers/framebuf.h"
#include "drivers/serial.h"
#include "util/print.h"
#include "util/assert.h"

#define TIMER_FREQ 50 /*Hz*/

static
void init()
{
    gdt_init();
    idt_init();
    irq_init();
    serial_init();
    timer_init(TIMER_FREQ);
    fb_init();
}

void kmain()
{
    init();

    fb_set_pos(24, 78);
    fb_write_str("hel\blo!\n");
    fb_write_str("world!\n");

//    __asm volatile ("int $0x3");
//    __asm volatile ("int $0x4");

    while (1) {
        for (u32_t i = 0; i < (1 << 20); ++i) {
            // do nothing, simulate delay
        }
    }
}
