#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "args.h"
#include "globals.h"
#include "eval.h"
#include "loop.h"

void eval_op(operation *op) {
    int i = 0;
    switch (op->opcode) {
        case ASGN:
            parse_two_args(op);
            *(long *)op->a1 = *(long *)op->a2;
            break;
        case RAND:
            parse_two_args(op);
            *(long *)op->a1 = rand() % *(long *)op->a2;
            break;
        case LDSTR:
            parse_two_args(op);
            if (*op->a1) free(*op->a1);
            *op->a1 = malloc(strlen((char *)op->a2) + 1);
            strcpy(*(char **)op->a1, (char *)op->a2);
            break;
        case GETI:
            parse_one_arg(op);
            fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
            *(long *)op->a1 = atoi(s_buff);
            break;
        case GETS:
            parse_one_arg(op);
            if (*op->a1) free(*op->a1);
            fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
            for (; s_buff[i]; i++) {
                if (s_buff[i] == '\n') {
                    s_buff[i] = 0x0;
                    break;
                }
            }
            *op->a1 = malloc(strlen(s_buff) + 1);
            strcpy(*(char **)op->a1, s_buff);
            break;
        case ADD:
            parse_three_args(op);
            *(long *)op->a1 = *(long *)op->a2 + *(long *)op->a3;
            break;
        case SUBTR:
            parse_three_args(op);
            *(long *)op->a1 = *(long *)op->a2 - *(long *)op->a3;
            break;
        case MUL:
            parse_three_args(op);
            *(long *)op->a1 = *(long *)op->a2 * *(long *)op->a3;
            break;
        case DIV:
            parse_three_args(op);
            *(long *)op->a1 = *(long *)op->a2 / *(long *)op->a3;
            break;
        case INCR:
            parse_one_arg(op);
            (*(long *)op->a1)++;
            break;
        case DECR:
            parse_one_arg(op);
            (*(long *)op->a1)--;
            break;
        case SWP:
            parse_two_args(op);
            (*(long *)op->a1) ^= (*(long *)op->a2);
            (*(long *)op->a2) ^= (*(long *)op->a1);
            (*(long *)op->a1) ^= (*(long *)op->a2);
            break;
        case BEGLP:
            *j_sp = op->lit;
            if (++j_sp > j_bp + GLOBAL_STACK_SIZE) {
                printf("Jump stack overflow\n");
                exit(-1);
            }
            break;
        case CONT:
            if (j_sp == j_bp) {
                break;
            }
            loop_backwards(op);
        case ENDLPEQ:
            if (j_sp == j_bp) {
                break;
            }
            parse_two_args(op);
            if (*(long *)op->a1 != *(long *)op->a2) {
                loop_backwards(op);
            } else {
                j_sp--;
            }
            break;
        case ENDLPNE:
            if (j_sp == j_bp) {
                break;
            }
            parse_two_args(op);
            if (*(long *)op->a1 == *(long *)op->a2) {
                loop_backwards(op);
            } else {
                j_sp--;
            }
            break;
        case ENDLPLT:
            if (j_sp == j_bp) {
                break;
            }
            parse_two_args(op);
            if (*(long *)op->a1 >= *(long *)op->a2) {
                loop_backwards(op);
            } else {
                j_sp--;
            }
            break;
        case ENDLPLE:
            if (j_sp == j_bp) {
                break;
            }
            parse_two_args(op);
            if (*(long *)op->a1 > *(long *)op->a2) {
                loop_backwards(op);
            } else {
                j_sp--;
            }
            break;
        case IFEQ:
            parse_two_args(op);
            if (*(long *)op->a1 != *(long *)op->a2) {
                while (memcmp(op->lit, "endif", 5) != 0) {
                    op->lit = strtok(NULL, " \n\t");
                }
            }
            break;
        case IFNE:
            parse_two_args(op);
            if (*(long *)op->a1 == *(long *)op->a2) {
                while (memcmp(op->lit, "endif", 5) != 0) {
                    op->lit = strtok(NULL, " \n\t");
                }
            }
            break;
        case IFLT:
            parse_two_args(op);
            if (*(long *)op->a1 >= *(long *)op->a2) {
                while (memcmp(op->lit, "endif", 5) != 0) {
                    op->lit = strtok(NULL, " \n\t");
                }
            }
            break;
        case IFLE:
            parse_two_args(op);
            if (*(long *)op->a1 > *(long *)op->a2) {
                while (memcmp(op->lit, "endif", 5) != 0) {
                    op->lit = strtok(NULL, " \n\t");
                }
            }
            break;
        case ENDIF:
            break;
        case PRINT:
            parse_one_arg(op);
            printf("%ld\n", *(long *)op->a1);
            break;
        case PRINTN:
            parse_one_arg(op);
            printf("%ld", *(long *)op->a1);
            break;
        case PRINT_S:
            parse_one_arg(op);
            printf("%s\n", *(char **)op->a1);
            break;
        case PRINTN_S:
            parse_one_arg(op);
            printf("%s", *(char **)op->a1);
            break;
        case DONE:
            for (; i < MAX_REGISTERS; i++) {
                if (*(s_registers + i)) {
                    free(*(s_registers + i));
                }
            }
            break;
        default:
            printf("Unknown command: %s\n", op->lit);
            exit(-1);
    }

    e_sp = e_bp;
}
