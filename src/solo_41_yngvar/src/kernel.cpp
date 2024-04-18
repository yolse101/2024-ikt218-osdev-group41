#include "libc/stdio.h"


extern "C" int kernel_main(void);

int kernel_main(){
    
    // Demonstrate print function
    print("\nDemonstration of print function:");

    // Create first and second string
    const char* message = "Hello, world!";
    const char* nextmessage = "Hello, UiAOS!";
    
    // Print first string
    print("\n");
    print("%f", message);

    // Print memory address of first string
    print("Memory address of first string: %p", message);

    // Print second string
    print("%f", nextmessage);

    // Print memory address of second string
    print("Memory address of second string: %p", nextmessage);
    print("\n");

    // Demonstrate key logger
    print("Keyboard logger:");
    print("\n");

    // Running loop
    while(1) {
    }

    return 0;
}