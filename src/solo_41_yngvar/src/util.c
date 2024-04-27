#include "libc/util.h"
#include "libc/stdint.h"
#include "libc/stdio.h"
#include "libc/stddef.h"
#include "kernel/memory.h"

// Function to copy memory from source to destination
void* memcpy(void* dest, const void* src, size_t count )
{
    char* dst8 = (char*)dest; // Cast destination pointer to char*
    char* src8 = (char*)src; // Cast source pointer to char*

    if (count & 1) { // If count is odd
        dst8[0] = src8[0]; // Copy one byte from source to destination
        dst8 += 1; // Increment destination pointer by one byte
        src8 += 1; // Increment source pointer by one byte
    }

    count /= 2; // Divide count by two to copy 2 bytes at a time
    while (count--) { // Loop through each pair of 2 bytes
        dst8[0] = src8[0]; // Copy first byte of pair from source to destination
        dst8[1] = src8[1]; // Copy second byte of pair from source to destination

        dst8 += 2; // Increment destination pointer by 2 bytes
        src8 += 2; // Increment source pointer by 2 bytes
    }

    return (void*)dest; // Return destination pointer
}

// Function to set block of memory with a 16-bit value
void* memset16 (void *ptr, uint16_t value, size_t num)
{
    uint16_t* p = ptr; // Cast pointer to uint16_t*
    while(num--)
        *p++ = value; // Set each 2-byte element to given value
    return ptr; // Return pointer to block of memory
}

// Function to set block of memory with byte value
void* memset (void * ptr, int value, size_t num )
{
    unsigned char* p=ptr; // Cast pointer to unsigned char*
    while(num--)
        *p++ = (unsigned char)value; // Set each byte to given value
    return ptr; // Return pointer to block of memory
}

// Calculate length of string
int strlen(char s[]) {
    int i = 0; // Declare i to hold length of string
    while (s[i] != '\0') ++i; // While there are chars in string, increment i
    return i; // Return number of chars in string
}

// Reverse string
void reverse(char s[]) {
    int c, i, j; // Declare variables
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) { // Iterate through half the string
        c = s[i]; // Temporarily hold char i
        s[i] = s[j]; // Move char j to position i
        s[j] = c; // Move char i to position j
    }
}

// Convert digits to characters
void int_to_ascii(int n, char str[]) {
    int i = 0; // Declare i
    int sign; // Declare sign
    if ((sign = n) < 0) n = -n; // If sign is negative, reverse sign
    do {str[i++] = n % 10 + '0'; // Extract digits from the right and store as ASCII in string
    } while ((n /= 10) > 0); // While there are digits in number
    if (sign < 0) str[i++] = '-'; // Add negative sign if number was negative
    str[i] = '\0'; // Add null terminator to string
    reverse(str); // Reverse string
}

// Handle panic scenario
void panic(const char *message) {
    asm volatile("cli"); // Disable interrupts
    print("KERNEL PANIC: %s\n", message); // Print error message
    while (1) { // Loop indefinately
    }
}
