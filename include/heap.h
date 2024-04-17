#ifndef SIMPLANG_HEAP_H
#define SIMPLANG_HEAP_H

#define HEADER_SIG 0x50485053

typedef struct {
    int sig;
    int size;
    ptr_type_e type;
} heap_hdr_t;

void init_heap();
void *simp_alloc(int size, ptr_type_e type);
void simp_free(char *ptr);

extern heap_hdr_t g_heap[GLOBAL_HEAP_SIZE];

#endif //SIMPLANG_HEAP_H
