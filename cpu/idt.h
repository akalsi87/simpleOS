/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file idt.h
 * \date Nov 02, 2016
 */

#ifndef SIMPLEOS_CPU_IDT_H
#define SIMPLEOS_CPU_IDT_H

#include "types.h"

void idt_init();

void interrupt_enable();

void interrupt_disable();

#endif/*SIMPLEOS_CPU_IDT_H*/
