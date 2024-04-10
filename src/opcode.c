#include <string.h>

#include "opcode.h"

void get_opcode(operation *op) {
    if (memcmp(op->lit, "ASGN", 4) == 0) {
        op->opcode = ASGN;
    } else if (memcmp(op->lit, "ADD", 3) == 0) {
        op->opcode = ADD;
    } else if (memcmp(op->lit, "SUBTR", 5) == 0) {
        op->opcode = SUBTR;
    } else if (memcmp(op->lit, "MUL", 3) == 0) {
        op->opcode = MUL;
    } else if (memcmp(op->lit, "DIV", 3) == 0) {
        op->opcode = DIV;
    } else if (memcmp(op->lit, "PRINT", 5) == 0) {
        op->opcode = PRINT;
    } else if (memcmp(op->lit, "DONE", 4) == 0) {
        op->opcode = DONE;
    } else {
        op->opcode = -1;
    }
}