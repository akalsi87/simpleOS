/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file perf.h
 * \date Nov 03, 2016
 */

#ifndef SIMPLEOS_UTIL_PERF_H
#define SIMPLEOS_UTIL_PERF_H

#include "cpu/types.h"

#define PERF_TIC(x) perf_start(#x)

#define PERF_TOC(x) perf_stop(#x)

void perf_start(char_t const* tag);

void perf_stop(char_t const* tag);

#endif/*SIMPLEOS_UTIL_PERF_H*/
