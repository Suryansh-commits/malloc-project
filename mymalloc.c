#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mymalloc.h"

// Define the metadata structure for each memory block 
typedef struct block_meta {
    size_t size;           // Size of the data region
    struct block_meta *next; // Pointer to the next block
    int free;              // 1 if free, 0 if in use
} block_meta_t;

// Define the size of the metadata header
#define META_SIZE sizeof(block_meta_t)

// Global base pointer to the start of the heap (head of the linked list) 
void *global_base = NULL;

/**
 * Finds the first free block that is large enough (First-Fit algorithm).
 */
static block_meta_t *find_free_block(block_meta_t **last, size_t size) {
    block_meta_t *current = (block_meta_t *)global_base;
    while (current &&!(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

/**
 * Requests new space from the kernel using sbrk(). [4, 3]
 */
static block_meta_t *request_space(block_meta_t *last, size_t size) {
    block_meta_t *block;
    block = (block_meta_t *)sbrk(0);
    void *request = sbrk(size + META_SIZE);
    
    // sbrk failed
    if (request == (void*)-1) {
        return NULL; 
    }

    if (last) {
        last->next = block;
    }
    
    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}

/**
 * Splits a block if it's larger than the requested size. 
 */
static void split_block(block_meta_t *block, size_t size) {
    // Only split if enough space remains for a new header + 8 bytes of data
    if (block->size >= size + META_SIZE + 8) {
        block_meta_t *new_block;
        // Calculate start of new free block
        new_block = (block_meta_t *)((char *)block + META_SIZE + size);
        new_block->size = block->size - size - META_SIZE;
        new_block->free = 1;
        new_block->next = block->next;
        
        // Update the original block
        block->size = size;
        block->next = new_block;
    }
}

/**
 * Merges adjacent free blocks (coalescing). 
 */
static void coalesce_blocks() {
    block_meta_t *current = (block_meta_t *)global_base;
    while (current && current->next) {
        if (current->free && current->next->free) {
            // Merge current->next into current
            current->size += current->next->size + META_SIZE;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

/**
 * Allocates a block of memory of at least 'size' bytes.
 */
void *malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    block_meta_t *block;
    
    if (!global_base) {
        // First call to malloc
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = block;
    } else {
        // Find an existing free block
        block_meta_t *last = (block_meta_t *)global_base;
        block = find_free_block(&last, size);
        
        if (block) {
            // Found a free block, try to split it
            split_block(block, size);
            block->free = 0;
        } else {
            // No free block found, request new space
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        }
    }

    // Return pointer to the data region, right after the metadata 
    return (void *)(block + 1);
}

/**
 * Frees a previously allocated memory block.
 */
void free(void *ptr) {
    if (!ptr) {
        return;
    }

    // Get the metadata header from the data pointer 
    block_meta_t *block = (block_meta_t *)ptr - 1;
    block->free = 1;

    // Coalesce adjacent free blocks 
    coalesce_blocks();
}

/**
 * Allocates memory for an array of 'nmemb' elements
 * of 'size' bytes each and initializes all bytes to zero.
 */
void *calloc(size_t nmemb, size_t size) {
    size_t total_size = nmemb * size;
    if (total_size == 0) {
        return NULL;
    }
    
    void *ptr = malloc(total_size);
    if (ptr) {
        // Initialize memory to zero
        memset(ptr, 0, total_size);
    }
    return ptr;
}

/**
 * Changes the size of the memory block pointed to by 'ptr'.
 */
void *realloc(void *ptr, size_t size) {
    if (!ptr) {
        // If ptr is NULL, realloc() behaves like malloc()
        return malloc(size);
    }

    if (size == 0) {
        // If size is 0, realloc() behaves like free()
        free(ptr);
        return NULL;
    }

    block_meta_t *block = (block_meta_t *)ptr - 1;
    if (block->size >= size) {
        // Block is large enough, split it if possible
        split_block(block, size);
        return ptr;
    }

    // Need to allocate a new, larger block and copy data
    void *new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL; // Allocation failed
    }

    // Copy data from old block to new block
    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    
    return new_ptr;
}
