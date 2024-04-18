#include "libc/gdt.h"

// Declare array of GDT entries
struct gdt_entry gdt[GDT_ENTRIES];

// Define GDT pointer containing size and location of GDT
struct gdt_ptr gdt_ptr; 

// Function to initialize GDT in kernel
void init_gdt() {

    // Set GDT limit (size)
    gdt_ptr.limit = sizeof(struct gdt_entry) * GDT_ENTRIES -1;

    // Set GDT base (location)
    gdt_ptr.base = (uint32_t) &gdt;

    // Create GDT entries and parameters for each entry (num, base, limit, access, granularity)
    gdt_set_gate(0, 0, 0, 0, 0);    // NULL segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    // Load GDT using the limit and base set for the pointer
    gdt_load(&gdt_ptr);

    // Flush to complete CPU transition to new GDT
    gdt_flush();
}

// Function to define GDT entries
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF); // setting the lower 16 bits
    gdt[num].base_middle = (base >> 16) & 0xFF; // setting the next 8 bits
    gdt[num].base_high = (base >> 24) & 0xFF; // setting the last 8 bits

    gdt[num].limit_low = (limit & 0xFFFF); // setting the lower 16 bits of the limit
    gdt[num].granularity = (limit >> 16) & 0x0F; // setting the 4 first granulatity bits (after the limit bits)

    gdt[num].granularity |= gran & 0xF0; // merging the last 4 granularity bits with granularity value
    gdt[num].access = access; // setting the access byte for this section
}

// Function to load new GDT using values for limit and base stored by the pointer
void gdt_load(struct gdt_ptr *gdt_ptr) {
    asm volatile("lgdt (%0)" : : "r" (gdt_ptr) : "memory"); // assembly to load GDT
}

// Function to flush GDT using inline assembly
void gdt_flush() {
    asm volatile(
        "movw $0x10, %%ax\n\t"           // moves the data segment selector to AX
        "movw %%ax, %%ds\n\t"            // loads DS with AX
        "movw %%ax, %%es\n\t"            // loads ES with AX
        "movw %%ax, %%fs\n\t"            // loads FS with AX
        "movw %%ax, %%gs\n\t"            // loads GS with AX
        "movw %%ax, %%ss\n\t"            // loads SS with AX
        "ljmp $0x08, $next\n\t"          // long jumps to the next instruction to update CS register
        "next:\n\t"
        :
        : 
        : "ax"
    );
}
