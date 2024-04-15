#include <stdio.h>

#include "defs.h"
#include "heap.h"

char g_heap[GLOBAL_HEAP_SIZE] = {};
heap_ptr_u heap_ptr = { .str_ptr = g_heap };
char *heap_end = g_heap + GLOBAL_HEAP_SIZE;

void init_heap() {
    heap_hdr_t hdr = { .sig = HEADER_SIG, .type = NONE, .size = GLOBAL_HEAP_SIZE - sizeof(hdr) };
    *heap_ptr.hdr_ptr = hdr;
}

void *simp_alloc(size_t size, ptr_type_e type) {
    printf("Checking sig... ");
    if (heap_ptr.hdr_ptr->sig != HEADER_SIG) heap_ptr.str_ptr = g_heap;
    printf("Done\n");
    printf("Finding next free space... ");
    while (heap_ptr.hdr_ptr->type != NONE || heap_ptr.hdr_ptr->size < size) {
        heap_ptr.str_ptr += heap_ptr.hdr_ptr->size + sizeof(*heap_ptr.hdr_ptr);
    }
    printf("Done\n");
    printf("Setting size... ");
    if (heap_ptr.hdr_ptr->size >= size + sizeof(*heap_ptr.hdr_ptr)) {
        heap_ptr.hdr_ptr->size = size;
    }
    printf("Done\n");
    printf("Setting type... ");
    heap_ptr.hdr_ptr->type = type;
    printf("Done\n");
    printf("Saving pointer... ");
    void *alloc_ptr = heap_ptr.hdr_ptr + 1;
    printf("Pointer location: %p\n", alloc_ptr);
    printf("Advancing pointer past allocated space... ");
    heap_ptr.str_ptr += heap_ptr.hdr_ptr->size + sizeof(*heap_ptr.hdr_ptr);
    printf("Done\n");
    printf("Checking for signature... ");
    if (heap_ptr.hdr_ptr->sig != HEADER_SIG) {
        printf("No signature found\n");
        printf("Setting signature and type... ");
        heap_hdr_t hdr = { .sig = HEADER_SIG, .type = NONE };
        printf("Done\n");
        printf("Checking if last allocated space...");
        if (!*(heap_ptr.str_ptr + sizeof(hdr) + 1)) {
            printf("Yes\n");
            hdr.size = heap_end - (char *)(heap_ptr.hdr_ptr + 1);
            printf("Writing header...");
            *heap_ptr.hdr_ptr = hdr;
        } else {
            printf("No\n");
        }
        printf("Done\n");
    }

    return alloc_ptr;
}

void simp_free(char *ptr) {
    printf("Running free...\n");
    heap_ptr.str_ptr = ptr;
    if ((--heap_ptr.hdr_ptr)->sig == HEADER_SIG) {
        printf("Freeing %p...", ptr);
        heap_ptr.hdr_ptr->type = NONE;
        printf("Done\n");
    }
}