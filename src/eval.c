#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "eval.h"
#include "heap.h"

void eval_op(operation *op) {
    static void *eval_jmp_tbl[] = {
        #define X(opcode, ...) &&opcode,
        OPCODE_TABLE
        #undef X
    };
    static void *asgn_jmp_tbl[] = {
        #define X(type, ld_fn, ...) ld_fn,
        PTR_TYPE_TABLE
        #undef X
    };
    static void *print_jmp_tbl[] = {
        #define X(type, ...) &&PRINT_##type,
        PTR_TYPE_TABLE
        #undef X
    };
    static void *printn_jmp_tbl[] = {
        #define X(type, ...) &&PRINTN_##type,
        PTR_TYPE_TABLE
        #undef X
    };
    static int foreach_counter = 0, foreach_max = 0;
    int i = 0;
    unsigned int nested_if = 0;
    dyn_ptr_t *args[] = {
        &op->a1,
        &op->a2,
        &op->a3,
    };

    for (; i < 3; i++) {
        if (op->target[i] == MAX_REGISTERS) {
            memcpy(args[i], &loop_next, sizeof(dyn_ptr_t));
        } else if (op->target[i] >= 0) {
            int idx = args[i]->idx;
            memcpy(args[i], &g_registers[op->target[i]], sizeof(dyn_ptr_t));
            args[i]->idx = idx;
        }
    }

    if (op->a1.type == ARR) {
        if (op->a1.idx >= op->a1.arr_size) {
            goto ARR_OOB_ERR;
        }
        op->a1.ptr.int_ptr += op->a1.idx;
    }

    if (op->a2.type == ARR) {
        if (op->a2.idx >= op->a2.arr_size) {
            goto ARR_OOB_ERR;
        }
        op->a2.ptr.int_ptr += op->a2.idx;
    }

    goto *eval_jmp_tbl[op->opcode];

    ASGN:
        goto *asgn_jmp_tbl[op->a2.type];
    NEWARR:
        if (op->a1.ptr.int_ptr) simp_free(op->a1.ptr.int_ptr);
        op->a1.type = ARR;
        op->a1.ptr.int_ptr = simp_alloc(sizeof(long) * *op->a2.ptr.int_ptr, ARR);
        op->a1.arr_size = (int)*op->a2.ptr.int_ptr;
        if (op->a2.transient) simp_free(op->a2.ptr.int_ptr);
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->a1, sizeof(dyn_ptr_t));
        goto END;
    RAND:
        if (foreach_max > 0) {
            *op->a1.ptr.int_ptr = rand() % *op->a2.ptr.int_ptr;
            if (op->a2.transient) simp_free(op->a2.ptr.int_ptr);
            goto END;
        }
        if (op->a1.ptr.int_ptr) simp_free(op->a1.ptr.int_ptr);
        op->a1.type = INT;
        op->a1.ptr.int_ptr = simp_alloc(sizeof(long), INT);
        *op->a1.ptr.int_ptr = rand() % *op->a2.ptr.int_ptr;
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->a1, sizeof(dyn_ptr_t));
        if (op->a2.transient) simp_free(op->a2.ptr.int_ptr);
        goto END;
    LDINT:
        if (foreach_max > 0) {
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr;
            if (op->a2.transient) simp_free(op->a2.ptr.int_ptr);
            goto END;
        }
        if (op->a1.type == INT && op->a1.ptr.int_ptr) simp_free(op->a1.ptr.int_ptr);
        if (op->a1.type == ARR) {
            *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr;
            if (op->a2.transient) simp_free(op->a2.ptr.int_ptr);
        } else {
            op->a1.type = INT;
            if (op->target[1] >= 0) {
                op->a1.ptr.int_ptr = simp_alloc(sizeof(long), INT);
                *op->a1.ptr.int_ptr = *op->a2.ptr.int_ptr;
            } else {
                op->a1.ptr.int_ptr = op->a2.ptr.int_ptr;
            }
            if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->a1, sizeof(dyn_ptr_t));
        }
        if (!pe) goto PRINT;
        goto END;
    LDSTR:
        if (op->a1.ptr.str_ptr) simp_free(op->a1.ptr.str_ptr);
        op->a1.type = STR;
        op->a1.ptr.str_ptr = op->a2.ptr.str_ptr;
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->a1, sizeof(dyn_ptr_t));
        if (!pe) goto PRINT;
        goto END;
    GETOPT:
        if (op->a1.type && *op->a1.ptr.int_ptr) {
            simp_free(op->a2.ptr.str_ptr);
            goto END;
        }
        printf("%s", op->a2.ptr.str_ptr);
        simp_free(op->a2.ptr.str_ptr);
    GET:
        if (op->a1.ptr.int_ptr) simp_free(op->a1.ptr.int_ptr);
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        if ('0' <= s_buff[0] && s_buff[0] <= '9') {
            op->a1.type = INT;
            op->a1.ptr.int_ptr = simp_alloc(sizeof(long), INT);
            *op->a1.ptr.int_ptr = atoi(s_buff);
        } else {
            for (; s_buff[i]; i++) {
                if (s_buff[i] == '\n') {
                    s_buff[i] = 0x0;
                    break;
                }
            }
            op->a1.type = STR;
            op->a1.ptr.str_ptr = simp_alloc(i, STR);
            strcpy(op->a1.ptr.str_ptr, s_buff);
        }
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->a1, sizeof(dyn_ptr_t));
        goto END;
    ADD:
        if (op->a1.type == STR || op->a2.type == STR) {
            printf("Type error: cannot add non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *op->a1.ptr.int_ptr + *op->a2.ptr.int_ptr;
            goto END;
        }
        *op->a1.ptr.int_ptr += *op->a2.ptr.int_ptr;
        if (!pe) goto PRINT;
        goto END;
    SUBTR:
        if (op->a1.type == STR || op->a2.type == STR) {
            printf("Type error: cannot subtract non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *op->a1.ptr.int_ptr - *op->a2.ptr.int_ptr;
            goto END;
        }
        *op->a1.ptr.int_ptr -= *op->a2.ptr.int_ptr;
        if (!pe) goto PRINT;
        goto END;
    MUL:
        if (op->a1.type == STR || op->a2.type == STR) {
            printf("Type error: cannot multiply non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *op->a1.ptr.int_ptr * *op->a2.ptr.int_ptr;
            goto END;
        }
        *op->a1.ptr.int_ptr *= *op->a2.ptr.int_ptr;
        if (!pe) goto PRINT;
        goto END;
    DIV:
        if (op->a1.type == STR || op->a2.type == STR) {
            printf("Type error: cannot divide non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *op->a1.ptr.int_ptr / *op->a2.ptr.int_ptr;
            goto END;
        }
        *op->a1.ptr.int_ptr /= *op->a2.ptr.int_ptr;
        if (!pe) goto PRINT;
        goto END;
    MOD:
        if (op->a1.type == STR || op->a2.type == STR) {
            printf("Type error: cannot take modulo non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *op->a1.ptr.int_ptr % *op->a2.ptr.int_ptr;
            goto END;
        }
        *op->a1.ptr.int_ptr %= *op->a2.ptr.int_ptr;
        if (!pe) goto PRINT;
        goto END;
    INCR:
        if (op->a1.type == STR) {
            printf("Type error: cannot increment non-numerical values\n");
            goto END;
        }
        (*op->a1.ptr.int_ptr)++;
        if (!pe) goto PRINT;
        goto END;
    DECR:
        if (op->a1.type == STR) {
            printf("Type error: cannot decrement non-numerical value\n");
            goto END;
        }
        (*op->a1.ptr.int_ptr)--;
        if (!pe) goto PRINT;
        goto END;
    SWP:
        if (op->a1.type == STR || op->a2.type == STR) {
            printf("Type error: cannot swap non-numerical values\n");
            goto END;
        }
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
    FOREACH:
        if (j_sp == j_bp || *(j_sp - 1) != pp) {
            *j_sp = pp;
            if (++j_sp > j_bp + GLOBAL_STACK_SIZE) {
                printf("Jump stack overflow\n");
                exit(-1);
            }
        }
        if (op->a2.type != ARR) {
            printf("No array to operate on\n");
            exit(-1);
        }
        foreach_max = op->a2.arr_size;
        op->a1.type = INT;
        op->a1.ptr.int_ptr = op->a2.ptr.int_ptr + foreach_counter++;
        memcpy(&g_registers[op->target[0]], &op->a1, sizeof(dyn_ptr_t));
        op->a1.ptr.int_ptr++;
        if (foreach_counter < foreach_max) {
            memcpy(&loop_next, &op->a1, sizeof(dyn_ptr_t));
        }
        goto END;
    CONT:
        if (j_sp == j_bp) {
            goto END;
        }
        pp = *(j_sp - 1);
        goto END;
    ENDLP:
        if (j_sp == j_bp) {
            goto END;
        }
        if (foreach_counter >= foreach_max) {
            foreach_counter = foreach_max = 0;
            j_sp--;
        } else {
            pp = *(j_sp - 1);
            pp--;
        }
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
        PRINT_STR:
            if (op->a1.transient) goto PRINT_TRANSIENT_STR;
            printf("%s\n", op->a1.ptr.str_ptr);
            goto END;
        PRINT_TRANSIENT_STR:
            printf("%s\n", op->a1.ptr.str_ptr);
            simp_free(op->a1.ptr.str_ptr);
            goto END;
        PRINT_INT:
            if (op->a1.transient) goto PRINT_TRANSIENT_INT;
            printf("%ld\n", *op->a1.ptr.int_ptr);
            goto END;
        PRINT_TRANSIENT_INT:
            printf("%ld\n", *op->a1.ptr.int_ptr);
            simp_free(op->a1.ptr.int_ptr);
            goto END;
        PRINT_ARR:
            goto PRINT_INT;
    PRINTN:
        goto *printn_jmp_tbl[op->a1.type];
        PRINTN_STR:
            if (op->a1.transient) goto PRINTN_TRANSIENT_STR;
            printf("%s", op->a1.ptr.str_ptr);
            goto END;
        PRINTN_TRANSIENT_STR:
            printf("%s", op->a1.ptr.str_ptr);
            simp_free(op->a1.ptr.str_ptr);
            goto END;
        PRINTN_INT:
            if (op->a1.transient) goto PRINTN_TRANSIENT_INT;
            printf("%ld", *op->a1.ptr.int_ptr);
            goto END;
        PRINTN_TRANSIENT_INT:
            printf("%ld\n", *op->a1.ptr.int_ptr);
            simp_free(op->a1.ptr.int_ptr);
            goto END;
        PRINTN_ARR:
            goto PRINTN_INT;
    ARR_OOB_ERR:
        printf("Out of bounds access error\n");
        if (pe) exit(-1);
        goto END;
    END: CMNT: DONE: ENDIF: PRINT_NONE: PRINTN_NONE: INVLD: NOP:
        return;
}
