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

// Simple print function for printing text strings and memory addresses to terminal
void print(const char* format, ...) {

    // Initialize variable arguments
    va_list args;
    va_start(args, format);

    // Iterate over the format string
    for (int i = 0; format[i] != '\0'; i++) {
        // Check for format specifier
        if (format[i] == '%') {
            // Skip to next character after '%'
            i++;

            // Format is a memory address
            if (format[i] == 'p') {

                // Retrieve next argument from list
                uintptr_t address = va_arg(args, uintptr_t);

                // Buffer to hold string
                char addressStr[sizeof(uintptr_t) * 2 + 1];

                // Convert address to string
                address_to_string(address, addressStr);

                // Print address char by char
                putchar('0'); // For correct memory address format
                putchar('x'); // This too

                // Print each address char using putchar()
                for (int j = 0; addressStr[j] != '\0'; j++) {
                    putchar(addressStr[j]);
                }
                
                // Add newline after printing
                putchar('\n');
            
            // Format is a string
            } else if (format[i] == 'f') {

                // Print each string char using putchar()
                const char* str = va_arg(args, const char*);
                while (*str) {
                    putchar(*str++);
                }
                
                // Add newline after printing
                putchar('\n');
            }
        
        // No format specifier found, print current char
        } else {
            putchar(format[i]);
        }
    }

    // Clean up variable arguments
    va_end(args);
}

#ifdef __cplusplus
}
#endif
