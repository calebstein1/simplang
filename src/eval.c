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
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr;
            break;
        case RAND:
            *op->a1.ptr.int_ptr = rand() % *op->a2.ptr.int_ptr;
            break;
        case LDSTR:
            *op->a1.ptr.str_ptr = op->a2.ptr.char_ptr;
            break;
        case GETI:
            fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
            *op->a1.ptr.int_ptr = atoi(s_buff);
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
            *op->a1.ptr.str_ptr = &g_heap[i];
            break;
        case ADD:
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr + *op->a3.ptr.int_ptr;
            break;
        case SUBTR:
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr - *op->a3.ptr.int_ptr;
            break;
        case MUL:
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr * *op->a3.ptr.int_ptr;
            break;
        case DIV:
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr / *op->a3.ptr.int_ptr;
            break;
        case MOD:
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr % *op->a3.ptr.int_ptr;
            break;
        case INCR:
            (*op->a1.ptr.int_ptr)++;
            break;
        case DECR:
            (*op->a1.ptr.int_ptr)--;
            break;
        case SWP:
            (*op->a1.ptr.int_ptr) ^= (*op->a2.ptr.int_ptr);
            (*op->a2.ptr.int_ptr) ^= (*op->a1.ptr.int_ptr);
            (*op->a1.ptr.int_ptr) ^= (*op->a2.ptr.int_ptr);
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
            if (*op->a1.ptr.int_ptr != *op->a2.ptr.int_ptr) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case ENDLPNE:
            if (j_sp == j_bp) {
                break;
            }
            if (*op->a1.ptr.int_ptr == *op->a2.ptr.int_ptr) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case ENDLPLT:
            if (j_sp == j_bp) {
                break;
            }
            if (*op->a1.ptr.int_ptr >= *op->a2.ptr.int_ptr) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case ENDLPLE:
            if (j_sp == j_bp) {
                break;
            }
            if (*op->a1.ptr.int_ptr > *op->a2.ptr.int_ptr) {
                pp = *(j_sp - 1);
            } else {
                j_sp--;
            }
            break;
        case IFEQ:
            nested_if++;
            if (*op->a1.ptr.int_ptr != *op->a2.ptr.int_ptr) {
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
            if (*op->a1.ptr.int_ptr == *op->a2.ptr.int_ptr) {
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
            if (*op->a1.ptr.int_ptr >= *op->a2.ptr.int_ptr) {
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
            if (*op->a1.ptr.int_ptr > *op->a2.ptr.int_ptr) {
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
            switch(op->a1.type) {
                case INT:
                    printf("%ld\n", *op->a1.ptr.int_ptr);
                    break;
                case STR:
                    printf("%s\n", *op->a1.ptr.str_ptr);
                    break;
                case CHAR:
                    printf("%s\n", op->a1.ptr.char_ptr);
                    break;
                default:
                    break;
            }
            break;
        case PRINTN:
            switch(op->a1.type) {
                case INT:
                    printf("%ld", *op->a1.ptr.int_ptr);
                    break;
                case STR:
                    printf("%s", *op->a1.ptr.str_ptr);
                    break;
                case CHAR:
                    // TODO: free string after printing
                    printf("%s", op->a1.ptr.char_ptr);
                    break;
                default:
                    break;
            }
            break;
        case DONE:
            break;
    }

    e_sp = e_bp;
}
