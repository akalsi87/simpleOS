/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file timer.c
 * \date Nov 03, 2016
 */

#include "timer.h"
#include "isr.h"
#include "drivers/ports.h"
#include "drivers/serial.h"
#include "util/print.h"

static u32_t count = 0;
static u32_t frequency = 0;

static
void timercbk(registers r)
{
    (void)r;
    char_t tick[DEC_PRINT_CHARS_U32];
    ++count;
    debugwritestr("[TIMER] ");
    bufprintdec(tick, count);
    debugwritestr(tick);
    debugwritestr(" @ ");
    bufprintdec(tick, frequency);
    debugwritestr(tick);
    debugwritestr("Hz\n");
}

void timerinit(u32_t freq)
{
    irqhandler(IRQ0, timercbk);

    frequency = freq;
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    u32_t divisor = 1193180 / frequency;

    // set frequency command
    portwriteb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    u8_t l = (u8_t)(divisor & 0xFF);
    u8_t h = (u8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    portwriteb(0x40, l);
    portwriteb(0x40, h);
}
