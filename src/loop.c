#include <string.h>

#include "defs.h"
#include "globals.h"
#include "loop.h"

void loop_backwards(operation *op) {
    int i = 0;
    for (; i < 3; i++) {
        while (*op->lit) {
            op->lit++;
        }
        *op->lit = ' ';
    }
    for (; op->lit >= *(j_sp - 1); op->lit--) {
        if (*op->lit) continue;
        *op->lit = ' ';
    }
    strtok(++op->lit, " \n\t");
}
