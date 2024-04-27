#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/gdt.h"
#include "libc/isr.h"
#include "libc/stdio.h"
#include "libc/keyboard.h"
#include "multiboot2.h"
#include "kernel/memory.h"
#include "kernel/pit.h"
#include "libc/util.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
   
    // Initiate GDT
    init_gdt();

    // Initiate IDT
    init_isr();

    // Initiate keyboard interrupt
    init_keyboard();

    // Test interrupts
    print("Interrupt testing, sending interrupt 0 and 1:\n");
    asm volatile ("int $0"); // Trigger interrupt 0
    asm volatile ("int $1"); // Trigger interrupt 1

    // Reenable interrupts
    asm volatile ("sti");

    // Initialize kernel memory manager
    init_kernel_memory(&end);

    // Initialize paging for memory management
    init_paging();

    // Print memory information
    print_memory_layout();

    // Initialize PIT
    init_pit(1000);

    print("Hello, World!\n");
    print("Pausing 10 seconds to let you read...\n");

    int i = 0;
    while(i < 1800000000){
    i++;        
    }

    return kernel_main();
}
