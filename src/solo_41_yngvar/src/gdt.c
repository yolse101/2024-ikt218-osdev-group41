#include "libc/gdt.h"

// Declaring array of GDT entries (GDT_ENTRIES set to 3 in "gdt.h")
struct gdt_entry gdt[GDT_ENTRIES];

// Defining GDT pointer containing size and location of GDT
struct gdt_ptr gdt_ptr; 

// Function to initialize the GDT in the kernel
void init_gdt() {

    // Setting the GDT limit (size)
    gdt_ptr.limit = sizeof(struct gdt_entry) * GDT_ENTRIES -1;

    // Setting the GDT base (location)
    gdt_ptr.base = (uint32_t) &gdt;

    // Creating GDT entries and parameters for each entry (num, base, limit, access, granularity)
    gdt_set_gate(0, 0, 0, 0, 0);    // NULL segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    // Loading GDT using the limit and base set for the pointer
    gdt_load(&gdt_ptr);

    // Flushing to complete CPU transition to new GDT
    gdt_flush((uint32_t)&gdt_ptr);
}

// Function to define GDT entries
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

// Function to load new GDT using values for limit and base stored by the pointer
void gdt_load(struct gdt_ptr *gdt_ptr) {
    asm volatile("lgdt (%0)" : : "r" (gdt_ptr) : "memory");
}

// Function to flush GDT, 
// moves data segment to AX and
// performs long jump to move intruction pointer and update CS register
void gdt_flush() {
    asm volatile(
        "movw $0x10, %%ax\n\t"           // Moves the data segment selector to AX
        "movw %%ax, %%ds\n\t"            // Loads DS with AX
        "movw %%ax, %%es\n\t"            // Loads ES with AX
        "movw %%ax, %%fs\n\t"            // Loads FS with AX
        "movw %%ax, %%gs\n\t"            // Loads GS with AX
        "movw %%ax, %%ss\n\t"            // Loads SS with AX
        "ljmp $0x08, $next\n\t"          // Long jumps to the next instruction
        "next:\n\t"
        :
        : 
        : "ax"
    );
}
