#include "kernel/memory.h"
#include "libc/stdio.h" 

static uint32_t* page_directory = 0; // Define pointer to page directory and initialize to 0
static uint32_t page_dir_loc = 0; // Define location of page directory and initialize to 0
static uint32_t* last_page = 0; // Define pointer to last page and initialize to 0


// Function to map virtual addresses to physical addresses
void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys)
{
    uint16_t id = virt >> 22; // Get upper 10 bits of virtual address to use as index in the page directory
    for(int i = 0; i < 1024; i++) // Loop through all 1024 page table entries
    {
        last_page[i] = phys | 3; // Set page table entry to physical address with present and write permissions set
        phys += 4096; // Increment physical address by page size of 4 KB
    }
    // Set page directory entry for virtual address to physical address of page table with present and write permissions set
    page_directory[id] = ((uint32_t)last_page) | 3; 
    last_page = (uint32_t *)(((uint32_t)last_page) + 4096); // Move to next page in memory
}

// Function to enable paging
void paging_enable()
{
    asm volatile("mov %%eax, %%cr3": :"a"(page_dir_loc)); // Load physical address of page directory into CR3 register
    asm volatile("mov %cr0, %eax"); // Load CR0 register into EAX register
    asm volatile("orl $0x80000000, %eax"); // Set paging enable bit in EAX register
    asm volatile("mov %eax, %cr0"); // Load EAX register into CR0 register to enable paging
}

// Function to initialize paging
void init_paging()
{
    print("Setting up paging\n");
    page_directory = (uint32_t*)0x400000; // Set page directory to start at 4 MB
    page_dir_loc = (uint32_t)page_directory; // Set physical address of the page directory
    last_page = (uint32_t *)0x404000; // Set last page to start at 4 MB + 4 KB
    for(int i = 0; i < 1024; i++) // Loop through all 1024 page directory entries
    {
        page_directory[i] = 0 | 2; // Set page directory entry to not present with supervisor level read/write permissions
    }
    paging_map_virtual_to_phys(0, 0); // Map first 4 MB of virtual memory to first 4 MB of physical memory
    paging_map_virtual_to_phys(0x400000, 0x400000); // Map next 4 MB of virtual memory to next 4 MB of physical memory
    paging_enable(); // Enable paging
    print("Paging was successfully enabled!\n");
}
