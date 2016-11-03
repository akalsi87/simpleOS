/*
Copyright (c) 2016 Aaditya Kalsi - All Rights Reserved.
*/

/**
 * \file gdt.c
 * \date Oct 30, 2016
 */

#include "gdt.h"

#include "types.h"

#include "util/assert.h"

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
typedef struct
{
    u16_t limit_low;           // The lower 16 bits of the limit.
    u16_t base_low;            // The lower 16 bits of the base.
    u8_t  base_middle;         // The next 8 bits of the base.
    u8_t  access;              // Access flags, determine what ring this segment can be used in.
    u8_t  granularity;
    u8_t  base_high;           // The last 8 bits of the base.
} gdt_entry_t;

STATIC_ASSERT(sizeof(gdt_entry_t) == 8, sizeof_gdt_entry_must_be_8);

typedef struct
{
    u16_t limit;               // The upper 16 bits of all selector limits.
    u16_t baselo;              // The address of the first gdt_entry_t struct.
    u16_t basehigh;
} gdt_ptr_t;

STATIC_ASSERT(sizeof(gdt_ptr_t) == 6, sizeof_gdt_ptr_must_be_6);

static gdt_entry_t ENTRIES[5];
static gdt_ptr_t   PTR;

// Set the value of one GDT entry.
static
void set_gate(i32_t num, u32_t base, u32_t limit, u8_t access, u8_t gran)
{
    ENTRIES[num].base_low    = (base & 0xFFFF);
    ENTRIES[num].base_middle = (base >> 16) & 0xFF;
    ENTRIES[num].base_high   = (base >> 24) & 0xFF;

    ENTRIES[num].limit_low   = (limit & 0xFFFF);
    ENTRIES[num].granularity = (limit >> 16) & 0x0F;

    ENTRIES[num].granularity|= gran & 0xF0;
    ENTRIES[num].access      = access;
}

// In ASM!
void gdt_flush(u32_t addr);

void gdt_init()
{
    u32_t* gdt_addr = (u32_t*)&(PTR.baselo);
    *gdt_addr = (u32_t)(void*)&ENTRIES[0];
    PTR.limit = 5*sizeof(gdt_entry_t)-1;

    set_gate(0, 0, 0, 0, 0);                // Null segment
    set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((u32_t)&PTR);
}
