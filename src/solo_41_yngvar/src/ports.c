#include "libc/ports.h"

// Read a byte from given port
uint8_t port_byte_in (uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

// Write a byte to given port
void port_byte_out (uint16_t port, uint8_t data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}
