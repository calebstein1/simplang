#ifndef SIMPLANG_HEAP_H
#define SIMPLANG_HEAP_H

#define HEADER_SIG 0x53504850

typedef struct {
    int sig;
    int size;
    ptr_type_e type;
} heap_hdr_t;

typedef union {
    char *str_ptr;
    long *int_ptr;
    heap_hdr_t *hdr_ptr;
} heap_ptr_u;

void init_heap();
void *simp_alloc(int size, ptr_type_e type);
void simp_free(char *ptr);

#endif //SIMPLANG_HEAP_H
