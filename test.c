#include "mymalloc.h"
#include <stdio.h>
#include <string.h>

int main() {
    printf("Starting custom memory allocator test...\n");

    // Test 1: Simple malloc and free
    printf("\nTest 1: Allocating 25 bytes...\n");
    char *str = (char *)malloc(25);
    if (str) {
        strcpy(str, "Hello Malloc!");
        printf("  > String: %s\n", str);
        free(str);
        printf("  > String freed.\n");
    }

    // Test 2: calloc (must initialize to zero)
    printf("\nTest 2: Allocating 10 ints with calloc...\n");
    int *arr = (int *)calloc(10, sizeof(int));
    if (arr) {
        int all_zero = 1;
        for (int i = 0; i < 10; i++) {
            if (arr[i] != 0) all_zero = 0;
        }
        printf("  > calloc block is all zeros: %s\n", all_zero ? "Yes" : "No");
        free(arr);
        printf("  > Array freed.\n");
    }

    // Test 3: realloc (changing size)
    printf("\nTest 3: Allocating and reallocating...\n");
    char *r_str = (char *)malloc(10);
    if (r_str) {
        strcpy(r_str, "Initial");
        printf("  > Initial string (10 bytes): %s\n", r_str);
        
        char *r_str_new = (char *)realloc(r_str, 50);
        if (r_str_new) {
            printf("  > Realloc successful. Old content retained: %s\n", r_str_new);
            strcat(r_str_new, " - Extended to 50 bytes.");
            printf("  > New string: %s\n", r_str_new);
            free(r_str_new);
        } else {
            printf("  > realloc failed!\n");
            free(r_str);
        }
    }

    // Test 4: Fragmentation & Coalescing Check
    printf("\nTest 4: Implicit Coalescing Test...\n");
    void *p1 = malloc(100);
    void *p2 = malloc(200);
    void *p3 = malloc(300);

    printf("  > Allocated p1(100), p2(200), p3(300)\n");

    free(p2);
    printf("  > Freed p2\n");

    free(p1);
    printf("  > Freed p1 (should coalesce with p2)\n");

    void *p4 = malloc(250);
    printf("  > Allocating 250 bytes after freeing p1+p2: %s\n",
            p4 ? "SUCCESS" : "FAILED");

    free(p3);
    free(p4);

    printf("\nAll tests completed.\n");
}
