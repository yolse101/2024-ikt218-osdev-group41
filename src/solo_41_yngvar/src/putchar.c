#include "libc/stdint.h"
#include "libc/stdio.h"


// Avoid name mangling when called from kernel.cpp
#ifdef __cplusplus
extern "C" {
#endif

// Declaring VGA variables
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

// Simple function to write char to VGA buffer
void putchar(char c) {

    // Current x position
    static int x = 0;

    // Current y position
    static int y = 0; 
    
    // Handle special characters
    switch (c) {

        // Newline char
        case '\n':

            // Skip to next line
            x = 0; 
            y++;
            break;

        // Tab char
        case '\t':

            // Move 4 spaces forward
            x += 4;
            break;

        // Backspace char
        case '\b':

            // If not at the beginning of line, go one space back
            if (x > 0) { 
                x--;
            
            // If at beginning of line, go to last position on previous line
            } else if (y > 0) {
                x = VGA_WIDTH - 1;
                y--;
            }
            
            // Clear current char
            VGA_BUFFER[y * VGA_WIDTH + x] = ' ' | 0x0700;
            break;

        default:
            // Write char to buffer and go to next position
            VGA_BUFFER[y * VGA_WIDTH + x] = c | 0x0700;
            x++;
            break;
    }

    // If at end of line, skip to start of next line
    if (x >= VGA_WIDTH) {
        x = 0;
        y++;
    }

    // Scroll if end of the screen
    if (y >= VGA_HEIGHT) {

        // Copy each row one row up
        for (int i = 0; i < VGA_HEIGHT - 1; i++) {
            for (int j = 0; j < VGA_WIDTH; j++) {
                VGA_BUFFER[i * VGA_WIDTH + j] = VGA_BUFFER[(i + 1) * VGA_WIDTH + j];
            }
        }

        // Clear last row
        for (int j = 0; j < VGA_WIDTH; j++) {
            VGA_BUFFER[(VGA_HEIGHT - 1) * VGA_WIDTH + j] = ' ' | 0x0700;
        }
        
        // Move to last row
        y = VGA_HEIGHT - 1;
    }
}

#ifdef __cplusplus
}
#endif
