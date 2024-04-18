#pragma once

#include "libc/stdint.h"

// Copy between memory locations
void mem_copy(char *source, char *dest, int nbytes);

// Set memory values
void mem_set(uint8_t *dest, uint8_t val, uint32_t len);

// Convert int to ASCII
void int_to_ascii(int n, char str[]);
