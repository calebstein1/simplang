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
    static int foreach_counter = 0, foreach_max = 0;
    int i, j;
    i = j = 0;
    unsigned int nested_if = 0;

    for (; i < op->arg_count; i++) {
        if (op->target[i] == MAX_REGISTERS) {
            memcpy(&op->arg_list[i], &loop_next, sizeof(dyn_ptr_t));
        } else if (op->target[i] >= 0) {
            int idx = op->arg_list[i].idx;
            bool arr_item = op->arg_list[i].arr_item;
            memcpy(&op->arg_list[i], &g_registers[op->target[i]], sizeof(dyn_ptr_t));
            op->arg_list[i].idx = idx;
            op->arg_list[i].arr_item = arr_item;
        }

        if (op->arg_list[i].type == ARR) {
            if (op->arg_list[i].idx >= op->arg_list[i].arr_size) {
                goto ARR_OOB_ERR;
            }
            op->arg_list[i].ptr += (op->arg_list[i].idx * sizeof(long));
        }
    }

    goto *eval_jmp_tbl[op->opcode];

    ASGN:
        goto *asgn_jmp_tbl[op->arg_list[1].type];
    RST:
        for (i = 0; i < op->arg_count; i++) {
            if (op->arg_list[i].type != INT) {
                printf("Type error: can only reset int types\n");
                continue;
            }
            *(long *)op->arg_list[i].ptr = 0;
        }
        goto END;
    NEWARR:
        if (op->arg_list[0].ptr) simp_free(op->arg_list[0].ptr);
        op->arg_list[0].type = ARR;
        op->arg_list[0].ptr = simp_alloc(sizeof(long) * *(long *)op->arg_list[1].ptr, ARR);
        op->arg_list[0].arr_size = (int)*(long *)op->arg_list[1].ptr;
        if (op->arg_list[1].transient) simp_free(op->arg_list[1].ptr);
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->arg_list[0], sizeof(dyn_ptr_t));
        goto END;
    RAND:
        if (foreach_max > 0 || op->arg_list[0].type == ARR) {
            *(long *)op->arg_list[0].ptr = rand() % *(long *)op->arg_list[1].ptr;
            if (op->arg_list[1].transient) simp_free(op->arg_list[1].ptr);
            goto END;
        }
        if (op->arg_list[0].ptr) simp_free(op->arg_list[0].ptr);
        op->arg_list[0].type = INT;
        op->arg_list[0].ptr = simp_alloc(sizeof(long), INT);
        *(long *)op->arg_list[0].ptr = rand() % *(long *)op->arg_list[1].ptr;
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->arg_list[0], sizeof(dyn_ptr_t));
        if (op->arg_list[1].transient) simp_free(op->arg_list[1].ptr);
        goto END;
    LDINT:
        if (foreach_max > 0) {
            *(long *)op->arg_list[0].ptr = *(long *)op->arg_list[1].ptr;
            if (op->arg_list[1].transient) simp_free(op->arg_list[1].ptr);
            goto END;
        }
        if (op->arg_list[0].type == INT && op->arg_list[0].ptr) simp_free(op->arg_list[0].ptr);
        if (op->arg_list[0].type == ARR) {
            *(long *)op->arg_list[0].ptr = *(long *)op->arg_list[1].ptr;
            if (op->arg_list[1].transient) simp_free(op->arg_list[1].ptr);
        } else {
            op->arg_list[0].type = INT;
            if (op->target[1] >= 0) {
                op->arg_list[0].ptr= simp_alloc(sizeof(long), INT);
                *(long *)op->arg_list[0].ptr = *(long *)op->arg_list[1].ptr;
            } else {
                op->arg_list[0].ptr = op->arg_list[1].ptr;
            }
            if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->arg_list[0], sizeof(dyn_ptr_t));
        }
        goto END;
    LDSTR:
        if (op->arg_list[0].ptr) simp_free(op->arg_list[0].ptr);
        op->arg_list[0].type = STR;
        op->arg_list[0].ptr = op->arg_list[1].ptr;
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->arg_list[0], sizeof(dyn_ptr_t));
        goto END;
    GET:
        if (op->arg_list[0].type && op->arg_list[1].type && *(long *)op->arg_list[0].ptr) {
            simp_free(op->arg_list[1].ptr);
            goto END;
        }
        if (op->arg_list[1].type) {
            printf("%s", (char *)op->arg_list[1].ptr);
            simp_free(op->arg_list[1].ptr);
        }
        if (op->arg_list[0].ptr) simp_free(op->arg_list[0].ptr);
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        if (('0' <= s_buff[0] && s_buff[0] <= '9') || s_buff[0] == '-') {
            op->arg_list[0].type = INT;
            op->arg_list[0].ptr = simp_alloc(sizeof(long), INT);
            *(long *)op->arg_list[0].ptr = atoi(s_buff);
        } else {
            for (; s_buff[i]; i++) {
                if (s_buff[i] == '\n') {
                    s_buff[i] = 0x0;
                    break;
                }
            }
            op->arg_list[0].type = STR;
            op->arg_list[0].ptr = simp_alloc(i, STR);
            strcpy(op->arg_list[0].ptr, s_buff);
        }
        if (op->target[0] >= 0) memcpy(&g_registers[op->target[0]], &op->arg_list[0], sizeof(dyn_ptr_t));
        goto END;
    ADD:
        if (op->arg_list[0].type == STR || op->arg_list[1].type == STR) {
            printf("Type error: cannot add non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *(long *)op->arg_list[0].ptr + *(long *)op->arg_list[1].ptr;
            goto END;
        }
        *(long *)op->arg_list[0].ptr += *(long *)op->arg_list[1].ptr;
        goto END;
    SUBTR:
        if (op->arg_list[0].type == STR || op->arg_list[1].type == STR) {
            printf("Type error: cannot subtract non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *(long *)op->arg_list[0].ptr - *(long *)op->arg_list[1].ptr;
            goto END;
        }
        *(long *)op->arg_list[0].ptr -= *(long *)op->arg_list[1].ptr;
        goto END;
    MUL:
        if (op->arg_list[0].type == STR || op->arg_list[1].type == STR) {
            printf("Type error: cannot multiply non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *(long *)op->arg_list[0].ptr * *(long *)op->arg_list[1].ptr;
            goto END;
        }
        *(long *)op->arg_list[0].ptr *= *(long *)op->arg_list[1].ptr;
        goto END;
    DIV:
        if (op->arg_list[0].type == STR || op->arg_list[1].type == STR) {
            printf("Type error: cannot divide non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *(long *)op->arg_list[0].ptr / *(long *)op->arg_list[1].ptr;
            goto END;
        }
        *(long *)op->arg_list[0].ptr /= *(long *)op->arg_list[1].ptr;
        goto END;
    MOD:
        if (op->arg_list[0].type == STR || op->arg_list[1].type == STR) {
            printf("Type error: cannot take modulo non-numerical values\n");
            goto END;
        }
        if (op->embedded) {
            i_buff = *(long *)op->arg_list[0].ptr % *(long *)op->arg_list[1].ptr;
            goto END;
        }
        *(long *)op->arg_list[0].ptr %= *(long *)op->arg_list[1].ptr;
        goto END;
    INCR:
        if (op->arg_list[0].type == STR) {
            printf("Type error: cannot increment non-numerical values\n");
            goto END;
        }
        (*(long *)op->arg_list[0].ptr)++;
        goto END;
    DECR:
        if (op->arg_list[0].type == STR) {
            printf("Type error: cannot decrement non-numerical value\n");
            goto END;
        }
        (*(long *)op->arg_list[0].ptr)--;
        goto END;
    SWP:
        if (op->arg_list[0].type == STR || op->arg_list[1].type == STR) {
            printf("Type error: cannot swap non-numerical values\n");
            goto END;
        }
        (*(long *)op->arg_list[0].ptr) ^= (*(long *)op->arg_list[1].ptr);
        (*(long *)op->arg_list[1].ptr) ^= (*(long *)op->arg_list[0].ptr);
        (*(long *)op->arg_list[0].ptr) ^= (*(long *)op->arg_list[1].ptr);
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
        if (op->arg_list[2].type != ARR) {
            printf("No array to operate on\n");
            exit(-1);
        }
        foreach_max = op->arg_list[2].arr_size;
        op->arg_list[0].type = INT;
        op->arg_list[0].ptr = (long *)op->arg_list[2].ptr + foreach_counter++;
        memcpy(&g_registers[op->target[0]], &op->arg_list[0], sizeof(dyn_ptr_t));
        if (foreach_counter < foreach_max) {
            op->arg_list[0].ptr += sizeof(long);
            memcpy(&loop_next, &op->arg_list[0], sizeof(dyn_ptr_t));
        }
        goto END;
    CONT:
        if (j_sp == j_bp) {
            goto END;
        }
        pp = *(j_sp - 1);
        if (foreach_max) pp--;
        goto END;
    BREAK:
        while (!(ENDLP <= op->opcode && op->opcode <= ENDLPGE)) {
            op = ++pp;
        }
        goto END;
    ENDLP:
        if (j_sp == j_bp) {
            goto END;
        }
        if (foreach_counter >= foreach_max) {
            foreach_counter = foreach_max = 0;
            j_sp--;
        } else if (!foreach_max) {
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
        if (*(long *)op->arg_list[0].ptr != *(long *)op->arg_list[1].ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPNE:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*(long *)op->arg_list[0].ptr == *(long *)op->arg_list[1].ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPLT:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*(long *)op->arg_list[0].ptr >= *(long *)op->arg_list[1].ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPLE:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*(long *)op->arg_list[0].ptr > *(long *)op->arg_list[1].ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPGT:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*(long *)op->arg_list[0].ptr <= *(long *)op->arg_list[1].ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    ENDLPGE:
        if (j_sp == j_bp) {
            goto END;
        }
        if (*(long *)op->arg_list[0].ptr < *(long *)op->arg_list[1].ptr) {
            pp = *(j_sp - 1);
        } else {
            j_sp--;
        }
        goto END;
    IFEQ:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr != *(long *)op->arg_list[1].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFNE:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr == *(long *)op->arg_list[1].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFLT:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr >= *(long *)op->arg_list[1].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFLE:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr > *(long *)op->arg_list[1].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFGT:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr <= *(long *)op->arg_list[1].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFGE:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr < *(long *)op->arg_list[1].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFRG:
        nested_if++;
        if (*(long *)op->arg_list[0].ptr < *(long *)op->arg_list[1].ptr || *(long *)op->arg_list[2].ptr < *(long *)op->arg_list[0].ptr) {
            goto SKIP_IF;
        }
        goto END;
    IFNR:
        nested_if++;
        if (*(long *)op->arg_list[1].ptr <= *(long *)op->arg_list[0].ptr && *(long *)op->arg_list[0].ptr <= *(long *)op->arg_list[2].ptr) {
            goto SKIP_IF;
        }
        goto END;
    SKIP_IF:
        while (nested_if) {
            op = ++pp;
            if (IFEQ <= op->opcode && op->opcode <= IFNR) {
                nested_if++;
            } else if (op->opcode == ENDIF) {
                nested_if--;
            }
        }
        goto END;
    PRINT:
        for (i = 0; i < op->arg_count; i++) {
            switch (op->arg_list[i].type) {
            case ARR:
                if (op->arg_list[i].arr_item) goto INT;
                printf("[");
                for (j = 0; j < op->arg_list[0].arr_size - 1; j++) {
                    printf("%ld, ", *((long *)op->arg_list[0].ptr + j));
                }
                printf("%ld]", *((long *)op->arg_list[0].ptr + j));
                break;
            case INT: INT:
                printf("%ld", *(long *)op->arg_list[i].ptr);
                break;
            case STR:
                printf("%s", (char *)op->arg_list[i].ptr);
                break;
            default:
                continue;
            }
            if (op->arg_list[i].transient) simp_free(op->arg_list[i].ptr);
        }
        printf("\n");
        goto END;
    ARR_OOB_ERR:
        printf("Out of bounds access error\n");
        if (pe) exit(-1);
        goto END;
    END: CMNT: DONE: ENDIF: INVLD:
        return;
}
