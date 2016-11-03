/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file isr.h
 * \date Nov 03, 2016
 */

#ifndef SIMPLEOS_CPU_ISR_H
#define SIMPLEOS_CPU_ISR_H

#include "types.h"

typedef struct
{
    u32_t ds;                                     // Data segment selector
    u32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    u32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers_t;

typedef void (*irq_t)(registers_t const*);

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void irq_init();

// idx E [0, 256)
// returns old handler and installs new handler
irq_t irq_set_handler(u8_t idx, irq_t h);

#endif/*SIMPLEOS_CPU_ISR_H*/
