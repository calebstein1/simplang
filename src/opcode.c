#include <string.h>

#include "defs.h"
#include "opcode.h"

#define X(opcode, lit) lit,
char *opcode_lit[] = {
        OPCODE_TABLE
};
#undef X

void get_opcode(operation *op) {
    int i = 0;
    for (; memcmp(opcode_lit[i], op->lit, strlen(op->lit)) != 0; i++) {
        if (i >= DONE) {
            op->opcode = -1;
            return;
        }
    }
    op->opcode = i;
}
