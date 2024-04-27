#include "kernel/pit.h"
#include "libc/ports.h"
#include "libc/isr.h"
#include "libc/util.h"
#include "libc/stdio.h"

// Number of timer ticks since system start
uint32_t tick = 0;

// Callback function for PIT interrupts
static void pit_callback(registers_t regs) {
    tick++;  // Increment tick counter on each PIT interrupt
}

// Initialize PIT to generate system ticks at specified frequency
void init_pit(uint32_t freq) {
    // Register PIT callback function as handler for IRQ0
    register_interrupt_handler(IRQ0, pit_callback);
    // Calculate divisor for given frequency
    uint32_t divisor = 1193180 / freq; // 1193180 Hz is PIT base frequency
    uint8_t low  = (uint8_t)(divisor & 0xFF);  // Extract low byte of divisor
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);  // Extract high byte of divisor
    // Configure PIT to repeat given frequency
    port_byte_out(0x43, 0x36); // Command port: Channel 0, Access mode: lobyte/hibyte, Op. mode: rate generator (0x36)
    port_byte_out(0x40, low);  // Send low byte of divisor to channel 0
    port_byte_out(0x40, high); // Send high byte of divisor to channel 0
}

// Get current system tick count
uint32_t get_current_tick() {
    return tick;
}

// Sleep function using busy waiting for specified number of milliseconds
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();  // Get current tick count
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;  // Calculate number of ticks to wait
    uint32_t elapsed_ticks = 0;  // Initialize elapsed tick counter
    while (elapsed_ticks < ticks_to_wait) {  // Loop until wait time is over
        while (get_current_tick() == start_tick + elapsed_ticks) {
        } // Busy wait: do nothing
        elapsed_ticks++;  // Increment elapsed tick count when current tick count changes
    }
}

// Sleep function using interrupts for specified number of milliseconds
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = get_current_tick();  // Get current tick count
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;  // Calculate number of ticks to wait
    uint32_t end_ticks = current_tick + ticks_to_wait;  // Calculate end tick count
    while (current_tick < end_ticks) {  // Loop until current tick count is less than end tick count
        __asm__ __volatile__("sti");  // Assembly instruction to enable interrupts
        __asm__ __volatile__("hlt");  // Assembly instruction to halt the CPU until the next interrupt
        current_tick = get_current_tick();  // Update current tick count after waking up from halt
    }
}
