#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "eval.h"
#include "heap.h"


void eval_op(operation *op) {
    static void *eval_jmp_tbl[] = {
        #define X(opcode, lit, parse_fn, eval_lbl) eval_lbl,
        OPCODE_TABLE
        #undef X
    };
    static void *print_jmp_tbl[] = {
        #define X(type, print_lbl, printn_lbl) print_lbl,
        PTR_TYPE_TABLE
        #undef X
    };
    static void *printn_jmp_tbl[] = {
        #define X(type, print_lbl, printn_lbl) printn_lbl,
        PTR_TYPE_TABLE
        #undef X
    };
    int i = 0;
    unsigned int nested_if = 0;
    char *str = NULL;

    goto *eval_jmp_tbl[op->opcode];

    ASGN:
        *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr;
        goto END;
    RAND:
        *op->a1.ptr.int_ptr = rand() % *op->a2.ptr.int_ptr;
        goto END;
    LDSTR:
        if (*op->a1.ptr.str_ptr) simp_free(*op->a1.ptr.str_ptr);
        *op->a1.ptr.str_ptr = op->a2.ptr.char_ptr;
        goto END;
    GETI:
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        *op->a1.ptr.int_ptr = atoi(s_buff);
        goto END;
    GETS:
        if (*op->a1.ptr.str_ptr) simp_free(*op->a1.ptr.str_ptr);
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        for (; s_buff[i]; i++) {
            if (s_buff[i] == '\n') {
                s_buff[i] = 0x0;
                break;
            }
        }
        str = simp_alloc(i, STR);
        strcpy(str, s_buff);
        *op->a1.ptr.str_ptr = str;
        goto END;
    ADD:
        *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr + *op->a3.ptr.int_ptr;
        goto END;
    SUBTR:
        *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr - *op->a3.ptr.int_ptr;
        goto END;
    MUL:
        *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr * *op->a3.ptr.int_ptr;
        goto END;
    DIV:
        *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr / *op->a3.ptr.int_ptr;
        goto END;
    MOD:
        *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr % *op->a3.ptr.int_ptr;
        goto END;
    INCR:
        (*op->a1.ptr.int_ptr)++;
        goto END;
    DECR:
        (*op->a1.ptr.int_ptr)--;
        goto END;
    SWP:
        (*op->a1.ptr.int_ptr) ^= (*op->a2.ptr.int_ptr);
        (*op->a2.ptr.int_ptr) ^= (*op->a1.ptr.int_ptr);
        (*op->a1.ptr.int_ptr) ^= (*op->a2.ptr.int_ptr);
        goto END;
    BEGLP:
        *j_sp = pp;
        if (++j_sp > j_bp + GLOBAL_STACK_SIZE) {
            printf("Jump stack overflow\n");
            exit(-1);
        }
        goto END;
    CONT:
        if (j_sp == j_bp) {
            goto END;
        }
        pp = *(j_sp - 1);
        goto END;
    ENDLPEQ:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*op->a1.ptr.int_ptr != *op->a2.ptr.int_ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPNE:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*op->a1.ptr.int_ptr == *op->a2.ptr.int_ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPLT:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*op->a1.ptr.int_ptr >= *op->a2.ptr.int_ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPLE:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*op->a1.ptr.int_ptr > *op->a2.ptr.int_ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPGT:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*op->a1.ptr.int_ptr <= *op->a2.ptr.int_ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPGE:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*op->a1.ptr.int_ptr < *op->a2.ptr.int_ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    IFEQ:
        nested_if++;
        if (*op->a1.ptr.int_ptr != *op->a2.ptr.int_ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFNE:
        nested_if++;
        if (*op->a1.ptr.int_ptr == *op->a2.ptr.int_ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFLT:
        nested_if++;
        if (*op->a1.ptr.int_ptr >= *op->a2.ptr.int_ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFLE:
        nested_if++;
        if (*op->a1.ptr.int_ptr > *op->a2.ptr.int_ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFGT:
        nested_if++;
        if (*op->a1.ptr.int_ptr <= *op->a2.ptr.int_ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFGE:
        nested_if++;
        if (*op->a1.ptr.int_ptr < *op->a2.ptr.int_ptr) {
            goto SKIP_IF;
        }
        goto END;
    SKIP_IF:
        while (nested_if) {
            op = ++pp;
            if (IFEQ <= op->opcode && op->opcode <= IFLE) {
                nested_if++;
            } else if (op->opcode == ENDIF) {
                nested_if--;
            }
        }
        goto END;
    PRINT:
        goto *print_jmp_tbl[op->a1.type];
        PRINT_INT:
            printf("%ld\n", *op->a1.ptr.int_ptr);
            goto END;
        PRINT_STR:
            printf("%s\n", *op->a1.ptr.str_ptr);
            goto END;
        PRINT_CHAR:
            printf("%s\n", op->a1.ptr.char_ptr);
            simp_free(op->a1.ptr.char_ptr);
            goto END;
    PRINTN:
        goto *printn_jmp_tbl[op->a1.type];
        PRINTN_INT:
            printf("%ld", *op->a1.ptr.int_ptr);
            goto END;
        PRINTN_STR:
            printf("%s", *op->a1.ptr.str_ptr);
            goto END;
        PRINTN_CHAR:
            printf("%s", op->a1.ptr.char_ptr);
            simp_free(op->a1.ptr.char_ptr);
            goto END;
    NOP:
        goto END;
    END:
        e_sp = e_bp;
}
