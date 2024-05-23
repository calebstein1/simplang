#ifndef SIMPLANG_HEAP_H
#define SIMPLANG_HEAP_H

#include <stdint.h>

#define HEADER_SIG 0x5053

typedef struct {
    uint16_t sig;
    uint16_t blocks;
    ptr_type_e type;
} heap_hdr_t;

void init_heap();
void *simp_alloc(int size, ptr_type_e type);
void simp_free(void *ptr);

extern heap_hdr_t g_heap[GLOBAL_HEAP_SIZE];

#endif //SIMPLANG_HEAP_H
