#pragma once

#define IDT_ENTRIES 256

#include "libc/stdint.h"

// Defining interrupt gate handler
typedef struct {
    uint16_t low_offset; // Lower 16 bits of address
    uint16_t sel; // Kernel segment selector
    uint8_t always0; // Always zero
    uint8_t flags; // Flags signaling gate type
    uint16_t high_offset; // Higher 16 bits of address
} __attribute__((packed)) idt_gate_t; // Avoid padding

// Defining pointer to interrupt handler array
typedef struct {
    uint16_t limit; // Size of the IDT in bytes - 1
    uint32_t base; // Base address of IDT
} __attribute__((packed)) idt_register_t; // Avoid padding

extern idt_gate_t idt[IDT_ENTRIES]; // External declaration of the IDT array
extern idt_register_t idt_reg; // External declaration of the IDT register

void set_idt_gate(int n, uint32_t handler); // Function to set an IDT gate
void set_idt(); // Function to set up the IDT
