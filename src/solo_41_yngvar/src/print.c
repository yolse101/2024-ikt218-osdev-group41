#include "libc/stdint.h"
#include "libc/stdio.h"
#include "libc/stdint.h"
#include "libc/stdarg.h"


// Avoid name mangling when called from kernel.cpp
#ifdef __cplusplus
extern "C" {
#endif

// Function to convert memory address into hexadecimal string
void address_to_string(uintptr_t address, char* buffer) {

    // Define characters representing hexadecimal digits
    const char* hexChars = "0123456789ABCDEF";

    // Iterate through address
    for (int i = sizeof(uintptr_t) * 2 - 1; i >= 0; i--) {

        // Extract the lowest 4 bits of address and convert to hexadecimal
        buffer[i] = hexChars[address & 0xF];

        // Shift the address to the right by 4 bits to process the next 4 bits
        address >>= 4;
    }
    // Null-terminate string to indicate end
    buffer[sizeof(uintptr_t) * 2] = '\0';
}

// Function to convert integer to string for decimal and hexadecimal
void itoa(unsigned int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdef"[tmp_value - value * base];
    } while (value);

    // Apply negative sign for base 10
    if (tmp_value < 0 && base == 10) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

// Print function
void print(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's': { // String
                    const char* str = va_arg(args, const char*);
                    while (*str) {
                        putchar(*str++);
                    }
                } break;
                case 'd': { // Decimal integer
                    int num = va_arg(args, int);
                    char numStr[12]; // Buffer to hold the integer string (including negative sign)
                    itoa(num, numStr, 10);
                    for (char* p = numStr; *p != '\0'; p++) {
                        putchar(*p);
                    }
                } break;
                case 'x': { // Hexadecimal integer
                    unsigned int num = va_arg(args, unsigned int);
                    char numStr[9]; // Buffer to hold the hexadecimal string
                    itoa(num, numStr, 16);
                    //putchar('0');
                    //putchar('x');
                    for (char* p = numStr; *p != '\0'; p++) {
                        putchar(*p);
                    }
                } break;
                case 'p': { // Pointer - handle as hexadecimal
                    void* ptr = va_arg(args, void*);
                    char ptrStr[19]; // Buffer to hold the pointer string
                    itoa((uintptr_t)ptr, ptrStr, 16);
                    putchar('0');
                    putchar('x');
                    for (char* p = ptrStr; *p != '\0'; p++) {
                        putchar(*p);
                    }
                } break;
                default: {
                    // Handle unexpected format specifier
                    putchar('%');
                    putchar(format[i]);
                } break;
            }
        } else {
            putchar(format[i]);
        }
    }

    va_end(args);
}

#ifdef __cplusplus
}
#endif
