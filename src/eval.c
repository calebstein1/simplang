#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "eval.h"

void eval_op(operation *op) {
    int i = 0;
    unsigned int nested_if = 0;

    switch (op->opcode) {
        case ASGN:
            *(long *)op->a1 = *(long *)op->a2;
            break;
        case RAND:
            *(long *)op->a1 = rand() % *(long *)op->a2;
            break;
        case LDSTR:
            *op->a1 = (char *)op->a2;
            break;
        case GETI:
            fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
            *(long *)op->a1 = atoi(s_buff);
            break;
        case GETS:
            fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
            for (; s_buff[i]; i++) {
                if (s_buff[i] == '\n') {
                    s_buff[i] = 0x0;
                    break;
                }
            }
            while (g_heap[i]) {
                i++;
                if(!g_heap[i] && g_heap[i + 1]) i++;
            }
            if (i > 0) i++;
            if (strlen(s_buff) + 1 >= GLOBAL_HEAP_SIZE) {
                printf("Heap overflow\n");
                exit(-1);
            }
            strcpy(&g_heap[i], s_buff);
            *op->a1 = &g_heap[i];
            break;
        case ADD:
            *(long *)op->a1 = *(long *)op->a2 + *(long *)op->a3;
            break;
        case SUBTR:
            *(long *)op->a1 = *(long *)op->a2 - *(long *)op->a3;
            break;
        case MUL:
            *(long *)op->a1 = *(long *)op->a2 * *(long *)op->a3;
            break;
        case DIV:
            *(long *)op->a1 = *(long *)op->a2 / *(long *)op->a3;
            break;
        case INCR:
            (*(long *)op->a1)++;
            break;
        case DECR:
            (*(long *)op->a1)--;
            break;
        case SWP:
            (*(long *)op->a1) ^= (*(long *)op->a2);
            (*(long *)op->a2) ^= (*(long *)op->a1);
            (*(long *)op->a1) ^= (*(long *)op->a2);
            break;
        case BEGLP:
            *j_sp = pp;
            if (++j_sp > j_bp + GLOBAL_STACK_SIZE) {
                printf("Jump stack overflow\n");
                exit(-1);
            }
            break;
        case CONT:
            if (j_sp == j_bp) {
                break;
            }
            pp = *(j_sp - 1);
            break;
        case ENDLPEQ:
            if (j_sp == j_bp) {
                break;
            }
            if (*(long *)op->a1 != *(long *)op->a2) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case ENDLPNE:
            if (j_sp == j_bp) {
                break;
            }
            if (*(long *)op->a1 == *(long *)op->a2) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case ENDLPLT:
            if (j_sp == j_bp) {
                break;
            }
            if (*(long *)op->a1 >= *(long *)op->a2) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case ENDLPLE:
            if (j_sp == j_bp) {
                break;
            }
            if (*(long *)op->a1 > *(long *)op->a2) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case IFEQ:
            nested_if++;
            if (*(long *)op->a1 != *(long *)op->a2) {
                while (nested_if) {
                    op = ++pp;
                    if (IFEQ <= op->opcode && op->opcode <= IFLE) {
                        nested_if++;
                    } else if (op->opcode == ENDIF) {
                        nested_if--;
                    }
                }
            }
            break;
        case IFNE:
            nested_if++;
            if (*(long *)op->a1 == *(long *)op->a2) {
                while (nested_if) {
                    op = ++pp;
                    if (IFEQ <= op->opcode && op->opcode <= IFLE) {
                        nested_if++;
                    } else if (op->opcode == ENDIF) {
                        nested_if--;
                    }
                }
            }
            break;
        case IFLT:
            nested_if++;
            if (*(long *)op->a1 >= *(long *)op->a2) {
                while (nested_if) {
                    op = ++pp;
                    if (IFEQ <= op->opcode && op->opcode <= IFLE) {
                        nested_if++;
                    } else if (op->opcode == ENDIF) {
                        nested_if--;
                    }
                }
            }
            break;
        case IFLE:
            nested_if++;
            if (*(long *)op->a1 > *(long *)op->a2) {
                while (nested_if) {
                    op = ++pp;
                    if (IFEQ <= op->opcode && op->opcode <= IFLE) {
                        nested_if++;
                    } else if (op->opcode == ENDIF) {
                        nested_if--;
                    }
                }
            }
            break;
        case ENDIF:
            break;
        case PRINT:
            printf("%ld\n", *(long *)op->a1);
            break;
        case PRINTN:
            printf("%ld", *(long *)op->a1);
            break;
        case PRINT_S:
            printf("%s\n", *(char **)op->a1);
            break;
        case PRINTN_S:
            printf("%s", *(char **)op->a1);
            break;
        case DONE:
            break;
        default:
            printf("Failed to eval operation %p\n", pp);
    }

    e_sp = e_bp;
}
