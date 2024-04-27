#pragma once

#include "libc/stdint.h"

void int_to_ascii(int n, char str[]); // Convert int to ASCII
void reverse(char s[]); // Helper function to reverse string
int strlen(char s[]); // Helper function to calculate string length
void panic(const char *message); // Panic functionm for error handling
