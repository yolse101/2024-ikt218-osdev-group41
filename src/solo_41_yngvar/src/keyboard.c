#include "libc/stdio.h"
#include "libc/isr.h"
#include "libc/keyboard.h"
#include "libc/ports.h"

// Assign keyboard_isr to IRQ1
void init_keyboard(void) {
    register_interrupt_handler(IRQ1, keyboard_isr);
}

// Print output from keyboard port
static void keyboard_isr() {
    output_keypress(port_byte_in(0x60));
}

// Output key press
void output_keypress(uint8_t scancode) {
    // Create key map
    char* keyMap[] = {"ERROR", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "+", "Backspace", "Tab",
                      "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "ENTER", "LCtrl", "A", "S",
                      "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LShift", "\\", "Z", "X", "C", "V",
                      "B", "N", "M", ",", ".", "/", "RShift", "Keypad *", "LAlt", "Spc"};
    int keyMapSize = sizeof(keyMap) / sizeof(char*); // Define size of map

    if (scancode < keyMapSize) { // Scancode is within map
        print(keyMap[scancode]); // Print pressed key
    } else if (scancode <= 0x7f) { // Scancode is outside map
        print("Unknown key"); // Print error message
    }
}
