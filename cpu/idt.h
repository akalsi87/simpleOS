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

void idtinit();

typedef struct
{
   u32_t ds;                                     // Data segment selector
   u32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   u32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
   u32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers;

typedef void (*irq)(registers);

// idx E [0, 256)
// returns old handler and installs new handler
irq irqhandler(u8_t idx, irq h);

#endif/*SIMPLEOS_CPU_IDT_H*/
