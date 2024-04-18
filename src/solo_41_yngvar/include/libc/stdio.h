#pragma once

// Avoid name mangling when called from kernel.cpp
#ifdef __cplusplus
extern "C" {
#endif

// Simple putchar function for use in print()
void putchar(char c);

// Simple print function for printing text strings and memory addresses to terminal
void print(const char* str, ...);

#ifdef __cplusplus
}
#endif
