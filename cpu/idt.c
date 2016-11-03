/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file idt.c
 * \date Nov 02, 2016
 */

#include "idt.h"
#include "isr.h" // for registers
#include "types.h"
#include "util/assert.h"
#include "util/debug.h"
#include "util/print.h"
#include "drivers/ports.h"

// ISRs
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
typedef struct
{
   u16_t low_off;               // The lower 16 bits of the handler function address
   u16_t sel;                   // The lower 16 bits of the base.
   u8_t  always0;               // The next 8 bits of the base.
   u8_t  flags;                 // Access flags, determine what ring this segment can be used in.
   u16_t high_off;              // The higher 16 bits of the handler function address
} idt_entry;

typedef struct
{
   u16_t limit;               // The upper 16 bits of all selector limits.
   u16_t baselo;              // The address of the first gdt_entry_t struct.
   u16_t basehigh;
} idt_ptr;

STATIC_ASSERT(sizeof(idt_entry) == 8, sizeof_idt_entry_must_be_8);
STATIC_ASSERT(sizeof(idt_ptr) == 6, sizeof_idt_ptr_must_be_6);

void idt_flush(u32_t idtptraddr);

static const u16_t KERNEL_CS = 0x08;
static idt_entry IDT[256];
static idt_ptr PTR;

static
void set_gate(i32_t n, u32_t handler)
{
    IDT[n].low_off = ((handler) & 0xFFFF);
    IDT[n].sel = KERNEL_CS;
    IDT[n].always0 = 0;
    IDT[n].flags = 0x8E; 
    IDT[n].high_off = ((handler >> 16) & 0xFFFF);
}

void idt_init()
{
    u32_t* idt_addr = (u32_t*)(&PTR.baselo);
    *idt_addr = (u32_t)(voidp_t)(&IDT);
    PTR.limit = sizeof(IDT)-1;

    set_gate(0, (u32_t)isr0);
    set_gate(1, (u32_t)isr1);
    set_gate(2, (u32_t)isr2);
    set_gate(3, (u32_t)isr3);
    set_gate(4, (u32_t)isr4);
    set_gate(5, (u32_t)isr5);
    set_gate(6, (u32_t)isr6);
    set_gate(7, (u32_t)isr7);
    set_gate(8, (u32_t)isr8);
    set_gate(9, (u32_t)isr9);
    set_gate(10, (u32_t)isr10);
    set_gate(11, (u32_t)isr11);
    set_gate(12, (u32_t)isr12);
    set_gate(13, (u32_t)isr13);
    set_gate(14, (u32_t)isr14);
    set_gate(15, (u32_t)isr15);
    set_gate(16, (u32_t)isr16);
    set_gate(17, (u32_t)isr17);
    set_gate(18, (u32_t)isr18);
    set_gate(19, (u32_t)isr19);
    set_gate(20, (u32_t)isr20);
    set_gate(21, (u32_t)isr21);
    set_gate(22, (u32_t)isr22);
    set_gate(23, (u32_t)isr23);
    set_gate(24, (u32_t)isr24);
    set_gate(25, (u32_t)isr25);
    set_gate(26, (u32_t)isr26);
    set_gate(27, (u32_t)isr27);
    set_gate(28, (u32_t)isr28);
    set_gate(29, (u32_t)isr29);
    set_gate(30, (u32_t)isr30);
    set_gate(31, (u32_t)isr31);

    // Remap the PIC
    // details: http://wiki.osdev.org/PIC
    port_write_byte(0x20, 0x11);
    port_write_byte(0xA0, 0x11);
    port_write_byte(0x21, 0x20);
    port_write_byte(0xA1, 0x28);
    port_write_byte(0x21, 0x04);
    port_write_byte(0xA1, 0x02);
    port_write_byte(0x21, 0x01);
    port_write_byte(0xA1, 0x01);
    port_write_byte(0x21, 0x0);
    port_write_byte(0xA1, 0x0); 

    // Install the IRQs
    set_gate(32, (u32_t)irq0);
    set_gate(33, (u32_t)irq1);
    set_gate(34, (u32_t)irq2);
    set_gate(35, (u32_t)irq3);
    set_gate(36, (u32_t)irq4);
    set_gate(37, (u32_t)irq5);
    set_gate(38, (u32_t)irq6);
    set_gate(39, (u32_t)irq7);
    set_gate(40, (u32_t)irq8);
    set_gate(41, (u32_t)irq9);
    set_gate(42, (u32_t)irq10);
    set_gate(43, (u32_t)irq11);
    set_gate(44, (u32_t)irq12);
    set_gate(45, (u32_t)irq13);
    set_gate(46, (u32_t)irq14);
    set_gate(47, (u32_t)irq15);

    idt_flush((u32_t)&PTR); // Load with ASM
}

char_t * EXCEPTIONS[] = {
    "Divide By 0",
    "Reserved",
    "Non Maskable Interrupt",
    "Breakpoint (INT3)",
    "Overflow (INT0)",
    "Out of Bounds",
    "Invalid Opcode",
    "Device not available (WAIT/FWAIT)",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "FPU error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t const* r)
{
    dbg_write_str("[ISR ");
    char_t dec[DEC_PRINT_CHARS_U32];
    buf_print_dec_32(dec, r->int_no);
    dbg_write_str(dec);
    dbg_write_str("] ");
    dbg_write_str(EXCEPTIONS[r->int_no]);
    dbg_write_str("\n");
}
