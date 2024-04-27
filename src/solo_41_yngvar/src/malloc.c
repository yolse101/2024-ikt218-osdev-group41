#include "kernel/memory.h"
#include "libc/stdio.h" 
#include "libc/util.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32

// Declare and set memory variables to 0
uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint32_t memory_used = 0;

// Initialize the kernel memory manager
void init_kernel_memory(uint32_t* kernel_end) {   
    last_alloc = kernel_end + 0x1000; // Set address for last allocation 4096 bytes beyond end of kernel
    heap_begin = last_alloc; // Set beginning of heap to last allocated address
    pheap_end = 0x400000; // Set permanent heap end to 4194304
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096); // Calculate beginning of permanent heap start
    heap_end = pheap_begin; // Set end of heap to start of permanent heap
    memset((char *)heap_begin, 0, heap_end - heap_begin); // Initialize heap memory to zero
    // Allocate memory for tracking page-aligned allocations and assign pointer
    pheap_desc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS); 
    print("Kernel heap starts at 0x%x\n", last_alloc); // Print starting address of heap
}

// Print the current memory layout
void print_memory_layout() { 
    print("Memory used: %d bytes\n", memory_used);
    print("Memory free: %d bytes\n", heap_end - heap_begin - memory_used);
    print("Heap size: %d bytes\n", heap_end - heap_begin);
    print("Heap start: 0x%x\n", heap_begin);
    print("Heap end: 0x%x\n", heap_end);
    print("PHeap start: 0x%x\n", pheap_begin);
    print("PHeap end: 0x%x\n", pheap_end);
}

// Free block of memory
void free(void *mem) {
    alloc_t *alloc = (mem - sizeof(alloc_t)); // Calculate start address
    memory_used -= alloc->size + sizeof(alloc_t); // Remove memory from memory_used
    alloc->status = 0; // Change allocation status to 0
}

// Free block of page-aligned memory
void pfree(void *mem) { 
    // If memory memory ourside heap, return
    if(mem < pheap_begin || mem > pheap_end) return;
    // Determine page ID
    uint32_t ad = (uint32_t)mem;
    ad -= pheap_begin; // Offset from the beginning of the heap
    ad /= 4096; // Convert the offset to an index by dividing by the page size
    // Set page descriptor to free
    pheap_desc[ad] = 0;
}

// Allocate block of page-aligned memory
char* pmalloc(size_t size) {
    // Loop through available list
    for(int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {
        if(pheap_desc[i]) continue; // Skip if page is allocated
        pheap_desc[i] = 1; // Mark as allocated
        // Print memory range of allocated page
        print("PAllocated from 0x%x to 0x%x\n", pheap_begin + i*4096, pheap_begin + (i+1)*4096);
        return (char *)(pheap_begin + i*4096); // Return pointer to beginning of allocated page
    }
    // If no page is available, print error message
    print("pmalloc: FATAL: failure!\n");
    return 0;
}

// Allocate block of memory
void* malloc(size_t size)
{
    // Return NULL if requested size is zero
    if(!size) return 0;
    // Start searching for available memory block from beginning of heap
    uint8_t *mem = (uint8_t *)heap_begin;
    while((uint32_t)mem < last_alloc)  // Ensure we do not go beyond the last allocation
    {
        alloc_t *a = (alloc_t *)mem;  // Point to current block's metadata
        print("mem=0x%x a={.status=%d, .size=%d}\n", mem, a->status, a->size);  // Debug output of current memory state
        // Jump to allocation label if block is end of heap
        if(!a->size)
            goto nalloc;
        // Skip current block if already allocated or not large enough
        if(a->status) {
            mem += a->size;  // Move to next block by adding size of current block
            mem += sizeof(alloc_t);  // Skip over block metadata
            mem += 4;  // Skip over padding or alignment
            continue;
        }
        // Check if current block is free and large enough
        if(a->size >= size)
        {
            a->status = 1;  // Mark block as allocated
            print("RE:Allocated %d bytes from 0x%x to 0x%x\n", size, mem + sizeof(alloc_t), mem + sizeof(alloc_t) + size);
            memset(mem + sizeof(alloc_t), 0, size);  // Zero out memory area to be returned
            memory_used += size + sizeof(alloc_t);  // Update used memory count
            return (char *)(mem + sizeof(alloc_t));  // Return memory address offset by size of metadata
        }
        // Move to next block if current is too small
        mem += a->size;
        mem += sizeof(alloc_t);
        mem += 4;
    }
    // Label to handle allocation at end of managed memory if no suitable block found
    nalloc:;
    if(last_alloc + size + sizeof(alloc_t) >= heap_end)
    {
        panic("Cannot allocate bytes! Out of memory.\n");  // Panic if no space left to allocate
    }
    alloc_t *alloc = (alloc_t *)last_alloc;  // Create new block at last allocation point
    alloc->status = 1;  // Mark block as allocated
    alloc->size = size;  // Set block size

    // Move last allocation pointer to account for new block size and metadata
    last_alloc += size;
    last_alloc += sizeof(alloc_t);
    last_alloc += 4;
    print("Allocated %d bytes from 0x%x to 0x%x\n", size, (uint32_t)alloc + sizeof(alloc_t), last_alloc);
    memory_used += size + 4 + sizeof(alloc_t);  // Update total used memory
    memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);  // Initialize allocated memory to zero
    return (char *)((uint32_t)alloc + sizeof(alloc_t));  // Return offset address as allocated memory
}
