#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"

// Avoid name mangling when called from kernel.cpp
#ifdef __cplusplus
extern "C" {
#endif

// Struct representing memory location
typedef struct {
    uint8_t status; // 0 = unallocated, 1 = allocated
    uint32_t size; // Size of memory in bytes
} alloc_t;

// Initiate kernel memory
void init_kernel_memory(uint32_t* kernel_end);

// Function declarations for paging operations
extern void init_paging(); // Initialize paging
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys); // Map virtual address to physical

// Function declarations for memory allocation
extern char* pmalloc(size_t size); // Allocate memory of given size with page alignment
extern void* malloc(size_t size); // Allocate memory of given size
extern void free(void *mem); // Free memory previously allocated

// Function declarations for memory manipulation
extern void* memcpy(void* dest, const void* src, size_t num ); // Copy num bytes from src to dest
extern void* memset (void * ptr, int value, size_t num ); // Set num bytes starting from ptr to value
extern void* memset16 (void *ptr, uint16_t value, size_t num); // Set num bytes starting from ptr to 16-bit value

// Other helper functions
void print_memory_layout();

#ifdef __cplusplus
}
#endif
