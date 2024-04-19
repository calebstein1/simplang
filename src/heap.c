#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "heap.h"

heap_hdr_t g_heap[GLOBAL_HEAP_SIZE] = {};
heap_hdr_t *heap_ptr = g_heap;
int max_heap_blocks = GLOBAL_HEAP_SIZE / sizeof(heap_hdr_t);

void init_heap() {
    heap_hdr_t hdr = { .sig = HEADER_SIG, .type = NONE, .blocks = max_heap_blocks };
    *heap_ptr = hdr;
}

void *simp_alloc(int size, ptr_type_e type) {
    uint16_t num_blocks = (size / sizeof(heap_hdr_t)) + 1;
    uint16_t offset_blocks = 0;
    heap_ptr = g_heap;
    while (heap_ptr->blocks < num_blocks || heap_ptr->type) {
        if (offset_blocks + num_blocks >= max_heap_blocks) {
            printf("Heap full\n");
            exit(-1);
        }
        offset_blocks += heap_ptr->blocks;
        heap_ptr += (heap_ptr->blocks + 1);
    }
    heap_ptr->blocks = num_blocks;
    heap_ptr->type = type;
    void *alloc_ptr = heap_ptr + 1;
    heap_ptr += (heap_ptr->blocks + 1);
    if (heap_ptr->sig != HEADER_SIG) {
        heap_hdr_t hdr = { .sig = HEADER_SIG, .type = NONE };
        if (!*(char *)((char *)(heap_ptr + 1) + 1)) {
            hdr.blocks = max_heap_blocks - offset_blocks;
        } else {
            heap_hdr_t *lookahead = heap_ptr;
            do {
                hdr.blocks++;
                lookahead++;
            } while (lookahead->sig != HEADER_SIG);
        }
        *heap_ptr = hdr;
    }

    return alloc_ptr;
}

void simp_free(void *ptr) {
    if (pe) return;

    heap_ptr = (heap_hdr_t *)ptr - 1;
    if (heap_ptr->sig != HEADER_SIG) return;

    heap_ptr->type = NONE;
    uint16_t extra_blocks = 0;
    heap_hdr_t *lookahead = heap_ptr;
    do {
        lookahead += (heap_ptr->blocks + 1);
        if (!lookahead->type) {
            extra_blocks += (lookahead->blocks + 1);
        }
    } while (!lookahead->type);

    heap_ptr->blocks += extra_blocks;
}