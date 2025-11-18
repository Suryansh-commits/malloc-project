#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h> // for size_t

// Function declarations that override the standard library
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

#endif // MY_MALLOC_H
