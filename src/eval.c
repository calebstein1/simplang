#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "args.h"
#include "globals.h"
#include "eval.h"
#include "loop.h"

void eval_op(operation *op) {
    switch (op->opcode) {
        case ASGN:
            parse_two_args(op);
            *op->a1 = *op->a2;
            break;
        case ADD:
            parse_three_args(op);
            *op->a1 = *op->a2 + *op->a3;
            break;
        case SUBTR:
            parse_three_args(op);
            *op->a1 = *op->a2 - *op->a3;
            break;
        case MUL:
            parse_three_args(op);
            *op->a1 = *op->a2 * *op->a3;
            break;
        case DIV:
            parse_three_args(op);
            *op->a1 = *op->a2 / *op->a3;
            break;
        case INCR:
            parse_one_arg(op);
            (*op->a1)++;
            break;
        case DECR:
            parse_one_arg(op);
            (*op->a1)--;
            break;
        case BEGLP:
            *j_sp = op->lit;
            if (++j_sp > j_bp + GLOBAL_STACK_SIZE) {
                printf("Jump stack overflow\n");
                exit(-1);
            }
            break;
        case ENDLPEQ:
            if (j_sp == j_bp) {
                break;
            }
            parse_two_args(op);
            if (*op->a1 != *op->a2) {
                loop_backwards(op);
            } else {
                j_sp--;
            }
            break;
        case PRINT:
            parse_one_arg(op);
            printf("%ld\n", *op->a1);
            break;
        case DONE:
            break;
        default:
            printf("Unknown command: %s\n", op->lit);
            exit(-1);
    }

    e_sp = e_bp;
}