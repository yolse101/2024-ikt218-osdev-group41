#pragma once

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Avoid name mangling when called from kernel.cpp
#ifdef __cplusplus
extern "C" {
#endif

// PIT (Programmable Interval Timer) related macros
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61
#define PIT_DEFAULT_DIVISOR 0x4E20 // 20000, which gives about 18.2 Hz (1193180 / 20000)

// IRQ0 related macros
#define PIC1_CMD_PORT 0x20  // Command port for master PIC
#define PIC1_DATA_PORT 0x21  // Data port for master PIC
#define PIC_EOI 0x20         // End-of-interrupt command code
 
// Custom sleep function constants
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000 // 1000 Hz
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / TARGET_FREQUENCY)

//uint32_t tick;  // Tick variable

void init_pit(uint32_t freq); // Initiate PIT
void sleep_interrupt(uint32_t milliseconds); // Sleep interrupt
void sleep_busy(uint32_t milliseconds); // Sleep busy
uint32_t get_current_tick();  // Get current tick

#ifdef __cplusplus
}
#endif