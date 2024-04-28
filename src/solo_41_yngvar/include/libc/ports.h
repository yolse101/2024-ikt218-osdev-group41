#pragma once

#include "libc/stdint.h"

// Avoid name mangling when called from kernel.cpp
#ifdef __cplusplus
extern "C" {
#endif

uint8_t port_byte_in (uint16_t port); // Reads a byte from input port and returns it
void port_byte_out (uint16_t port, uint8_t data); // Writes a byte to output port

#ifdef __cplusplus
}
#endif