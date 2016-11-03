/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file perf.c
 * \date Nov 03, 2016
 */

#include "perf.h"
#include "assert.h"
#include "debug.h"
#include "mem.h"
#include "print.h"
#include "cpu/timer.h"


static char_t const* last_tag = 0;
static u32_t last_tick = 0;

void perf_start(char_t const* tag)
{
	last_tag = tag;
	last_tick = timer_get_tick();
}

void perf_stop(char_t const* tag)
{
	u32_t diff = timer_get_tick() - last_tick;
	u32_t freq = timer_get_freq();
	ASSERT(str_compare(tag, last_tag) == 0);
	last_tick = 0;
	last_tag = 0;
	u32_t sec = ((1000 * 1000 * diff) / freq);
	dbg_write_str("[PERF] ");
	dbg_write_str(tag);
	dbg_write_str(": ");
	{// print time
		char_t dec[DEC_PRINT_CHARS_U32];
		buf_print_dec_32(dec, sec);
		dbg_write_str(dec);
	}
	dbg_write_str(" us.\n");
}
