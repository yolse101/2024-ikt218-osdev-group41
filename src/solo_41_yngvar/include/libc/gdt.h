#ifndef GDT_H
#define GDT_H
#define GDT_ENTRIES 3

#include "libc/stdint.h"


// Defining the GDT entry struct
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// creating GDT pointer struct
struct gdt_ptr {
    uint16_t limit; // size of GDT in bytes
    uint32_t base; // location in memory
} __attribute__((packed));

void init_gdt();
void gdt_load(struct gdt_ptr *gdt_ptr);
void gdt_flush();
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);


#endif // GDT_H

