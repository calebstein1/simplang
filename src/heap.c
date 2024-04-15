#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "heap.h"

char g_heap[GLOBAL_HEAP_SIZE] = {};
heap_ptr_u heap_ptr = { .str_ptr = g_heap };
int max_heap_blocks = GLOBAL_HEAP_SIZE / sizeof(heap_hdr_t);

void init_heap() {
    heap_hdr_t hdr = { .sig = HEADER_SIG, .type = NONE, .size = max_heap_blocks };
    *heap_ptr.hdr_ptr = hdr;
}

void *simp_alloc(int size, ptr_type_e type) {
    int num_blocks = (size / sizeof(heap_hdr_t)) + 1;
    int offset_blocks = 0;
    heap_ptr.str_ptr = g_heap;
    while (heap_ptr.hdr_ptr->size < num_blocks || heap_ptr.hdr_ptr->type != NONE) {
        if (offset_blocks + num_blocks >= max_heap_blocks) {
            printf("Heap full\n");
            exit(-1);
        }
        offset_blocks += heap_ptr.hdr_ptr->size;
        heap_ptr.hdr_ptr += (heap_ptr.hdr_ptr->size + 1);
    }
    heap_ptr.hdr_ptr->size = num_blocks;
    heap_ptr.hdr_ptr->type = type;
    void *alloc_ptr = heap_ptr.hdr_ptr + 1;
    heap_ptr.hdr_ptr += (heap_ptr.hdr_ptr->size + 1);
    if (heap_ptr.hdr_ptr->sig != HEADER_SIG) {
        heap_hdr_t hdr = { .sig = HEADER_SIG, .type = NONE };
        if (!*(heap_ptr.str_ptr + sizeof(hdr) + 1)) {
            hdr.size = max_heap_blocks - ((heap_ptr.str_ptr - g_heap) / sizeof(heap_hdr_t));
            *heap_ptr.hdr_ptr = hdr;
        }
    }

    return alloc_ptr;
}

void simp_free(char *ptr) {
    if (pe) return;

    heap_ptr.str_ptr = ptr - sizeof(heap_hdr_t);
    if (heap_ptr.hdr_ptr->sig != HEADER_SIG) return;

    heap_ptr.hdr_ptr->type = NONE;
    int extra_blocks = 0;
    heap_hdr_t *lookahead = heap_ptr.hdr_ptr;
    do {
        lookahead += (heap_ptr.hdr_ptr->size + 1);
        if (lookahead->type == NONE) {
            lookahead->sig = 0x0;
            extra_blocks += (lookahead->size + 1);
        }
    } while (lookahead->type == NONE);

    heap_ptr.hdr_ptr->size += extra_blocks;
}