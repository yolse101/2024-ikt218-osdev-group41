#pragma once

#include "libc/stdint.h"

unsigned char port_byte_in (uint16_t port); // Reads a byte from input port and returns it
void port_byte_out (uint16_t port, uint8_t data); // Writes a byte to output port
unsigned short port_word_in (uint16_t port); // Reads 2 bytes from input port and returns them
void port_word_out (uint16_t port, uint16_t data); // Writes 2 bytes to output port
