#include "libc/idt.h"

idt_gate_t idt[IDT_ENTRIES]; // Declare IDT
idt_register_t idt_reg; // Declare IDT register

// Set up entry in IDT
void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = (uint16_t)((handler) & 0xFFFF); // Lowest 16 bits
    idt[n].sel = 0x08; // Kernel segment selector
    idt[n].always0 = 0; // Always zero
    idt[n].flags = 0x8E; // Flags indicating gate type
    idt[n].high_offset = (uint16_t)(((handler) >> 16) & 0xFFFF); // Highest 16 bits
}

// Set up IDT register
void set_idt() {
    idt_reg.base = (uint32_t) &idt; // IDT base address
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1; // IDT limit
    asm volatile("lidtl (%0)" : : "r" (&idt_reg)); // Load IDT register from above values
}
