#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "globals.h"
#include "args.h"

void parse_op(operation *op) {
    switch (op->opcode) {
        case ASGN:
            parse_two_args(op);
            break;
        case RAND:
            parse_two_args(op);
            break;
        case LDSTR:
            parse_two_args(op);
            break;
        case GETI:
            parse_one_arg(op);
            break;
        case GETS:
            parse_one_arg(op);
            break;
        case ADD:
            parse_three_args(op);
            break;
        case SUBTR:
            parse_three_args(op);
            break;
        case MUL:
            parse_three_args(op);
            break;
        case DIV:
            parse_three_args(op);
            break;
        case INCR:
            parse_one_arg(op);
            break;
        case DECR:
            parse_one_arg(op);
            break;
        case SWP:
            parse_two_args(op);
            break;
        case BEGLP:
            break;
        case CONT:
            break;
        case ENDLPEQ:
            parse_two_args(op);
            break;
        case ENDLPNE:
            parse_two_args(op);
            break;
        case ENDLPLT:
            parse_two_args(op);
            break;
        case ENDLPLE:
            parse_two_args(op);
            break;
        case IFEQ:
            parse_two_args(op);
            break;
        case IFNE:
            parse_two_args(op);
            break;
        case IFLT:
            parse_two_args(op);
            break;
        case IFLE:
            parse_two_args(op);
            break;
        case ENDIF:
            break;
        case PRINT:
            parse_one_arg(op);
            break;
        case PRINTN:
            parse_one_arg(op);
            break;
        case PRINT_S:
            parse_one_arg(op);
            break;
        case PRINTN_S:
            parse_one_arg(op);
            break;
        case DONE:
            break;
    }
    if (!pe) return;

    memcpy(pe, op, sizeof(operation));
    pe++;
}
