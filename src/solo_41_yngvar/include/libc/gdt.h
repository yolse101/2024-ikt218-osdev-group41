#pragma once

#define GDT_ENTRIES 3 // setting the number of entries in current GDT

#include "libc/stdint.h"


// Defining the structure for GDT entries
struct gdt_entry {
    uint16_t limit_low;    // lower 16 bits of segment limit
    uint16_t base_low;     // lower 16 bits of segment base address
    uint8_t base_middle;   // next 8 bits of segment base address
    uint8_t access;        // access byte
    uint8_t granularity;   // granularity byte
    uint8_t base_high;     // upper 8 bits of segment base address
} __attribute__((packed));  // avoid padding


// Creating GDT pointer struct
struct gdt_ptr {
    uint16_t limit; // size of GDT in bytes
    uint32_t base; // location in memory
} __attribute__((packed)); // avoid padding

// Skeleton functions
void init_gdt(); // initiate GDT in kernel's main
void gdt_load(struct gdt_ptr *gdt_ptr); // load GDT inside init_gdt
void gdt_flush(); // transition to loaded GDT by far jumping and moving addresses

// Setting GDT entries 
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
