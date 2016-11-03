/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file timer.h
 * \date Nov 03, 2016
 */

#ifndef SIMPLEOS_CPU_TIMER_H
#define SIMPLEOS_CPU_TIMER_H

#include "types.h"

void timer_init(u32_t freq);

u32_t timer_get_freq();

u32_t timer_get_tick();

#endif/*SIMPLEOS_CPU_TIMER_H*/
