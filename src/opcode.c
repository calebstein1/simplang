#include <string.h>

#include "opcode.h"

void get_opcode(operation *op) {
    if (memcmp(op->lit, "asgn", 4) == 0) {
        op->opcode = ASGN;
    } else if (memcmp(op->lit, "add", 3) == 0) {
        op->opcode = ADD;
    } else if (memcmp(op->lit, "subtr", 5) == 0) {
        op->opcode = SUBTR;
    } else if (memcmp(op->lit, "mul", 3) == 0) {
        op->opcode = MUL;
    } else if (memcmp(op->lit, "div", 3) == 0) {
        op->opcode = DIV;
    } else if (memcmp(op->lit, "incr", 4) == 0) {
        op->opcode = INCR;
    } else if (memcmp(op->lit, "decr", 4) == 0) {
        op->opcode = DECR;
    } else if (memcmp(op->lit, "beglp", 5) == 0) {
        op->opcode = BEGLP;
    } else if (memcmp(op->lit, "endlpeq", 7) == 0) {
        op->opcode = ENDLPEQ;
    } else if (memcmp(op->lit, "print", 5) == 0) {
        op->opcode = PRINT;
    } else if (memcmp(op->lit, "done", 4) == 0) {
        op->opcode = DONE;
    } else {
        op->opcode = -1;
    }
}