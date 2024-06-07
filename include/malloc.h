#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>

#define malloc_or_fail(v, n, s) if (!(v = malloc(n * s))) { perror("malloc"); exit(-1); }
#define calloc_or_fail(v, n, s) if (!(v = calloc(n, s))) { perror("calloc"); exit(-1); }
#define realloc_or_fail(v, n, s) if (!(v = realloc(v, n * s))) { perror("realloc"); exit(-1); }

#endif