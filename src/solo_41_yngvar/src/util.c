#include "libc/util.h"

// Convert digits to characters
void int_to_ascii(int n, char str[]) {
    int i = 0;
    int sign;
    do {str[i++] = n % 10 + '0'; // Convert rightmost digit to character and store it
        n /= 10; // Move to next digit
    } while (n > 0); // While there are digits left
    str[i] = '\0'; // Add null terminator to mark end of string
}
