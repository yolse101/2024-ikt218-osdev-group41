#pragma once

static void keyboard_isr(); // ISR for keyboard, handles keyboard interrupts
void init_keyboard(void); // Initiate keyboard interrupts by pairing IRQ1 with keyboard_isr()
