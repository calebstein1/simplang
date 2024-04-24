#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "eval.h"
#include "heap.h"

char *opcode_lit[] = {
    #define X(opcode, lit, ...) lit,
    OPCODE_TABLE
    #undef X
};

void get_opcode(operation *op, char *tok, char **tok_pos) {
    int i = 1, l = 0;
    char *lit = strtok_r(tok, " \n", tok_pos);
    int lit_len = strlen(lit);
    for (; memcmp(opcode_lit[i], lit, (l = strlen(opcode_lit[i])) > lit_len ? l : lit_len) != 0; i++) {
        if (i >= DONE) {
            op->opcode = INVLD;

            printf("Invalid instruction: %s\n", lit);
            if (pe) exit(-1);

            return;
        }
    }
    op->opcode = i;
}

void parse_op(operation *op, char **tok_pos) {
    static char *reg_lbls[MAX_REGISTERS] = {};

    char *cur_arg = NULL;
    int i, j, num_args;
    j = num_args = 0;
    dyn_ptr_t *args[] = {
        &op->a1,
        &op->a2,
        &op->a3,
    };

    static void *parse_fn_tbl[] = {
        #define X(opcode, lit, parse_fn, ...) parse_fn,
        OPCODE_TABLE
        #undef X
    };

    goto *parse_fn_tbl[op->opcode];

    parse_three_args:
        ++num_args;
    parse_two_args:
        ++num_args;
    parse_one_arg:
        ++num_args;

    do {
        cur_arg = strtok_r(NULL, " \t", tok_pos);
        if (memcmp(cur_arg, "next", 4) == 0) {
            op->target[j] = MAX_REGISTERS;
        } else if (('a' <= *cur_arg && *cur_arg <= 'z') || ('A' <= *cur_arg && *cur_arg <= 'Z')) {
            int l, arg_len = strlen(cur_arg);
            i = l = 0;

            if (cur_arg[arg_len - 1] == '\n') {
                cur_arg[--arg_len] = 0x0;
            }
            if (cur_arg[arg_len - 1] == ']') {
                cur_arg[--arg_len] = 0x0;
                while (cur_arg[--arg_len] != '[') {}
                cur_arg[arg_len] = 0x0;
                args[j]->idx = atoi(cur_arg + arg_len + 1);
            }

            for (; i < MAX_REGISTERS; i++) {
                if (!reg_lbls[i]) {
                    reg_lbls[i] = simp_alloc(arg_len, STR);
                    strcpy(reg_lbls[i], cur_arg);
                    op->target[j] = i;
                    break;
                }
                if (memcmp(cur_arg, reg_lbls[i], (l = strlen(reg_lbls[i])) > arg_len ? l : arg_len) == 0) {
                    op->target[j] = i;
                    break;
                }
            }
            if (i == MAX_REGISTERS) {
                printf("Too many variables assigned\n");
                if (pe) exit(-1);
            }
        } else if (*cur_arg == '"') {
            i = 0;
            cur_arg++;
            while (*cur_arg != '"') {
                if (!*cur_arg) {
                    *cur_arg = ' ';
                }
                if (i >= GLOBAL_BUFF_SIZE - 1) {
                    printf("String buffer overflow\n");
                    if (pe) exit(-1);
                    return;
                }
                s_buff[i++] = *cur_arg++;
            }
            s_buff[i] = 0x0;
            args[j]->type = STR;
            args[j]->transient = true;
            args[j]->ptr.str_ptr = simp_alloc(i, STR);
            strcpy(args[j]->ptr.str_ptr, s_buff);
            strtok(cur_arg, " \t");
        } else if (*cur_arg == '(') {
            operation embedded_op = { .embedded = true, .target = { -1, -1, -1 } };
            char *embedded_tok_r = NULL;
            i = 0;
            cur_arg++;
            while (*cur_arg != ')') {
                if (!*cur_arg) {
                    *cur_arg = ' ';
                }
                if (i >= GLOBAL_BUFF_SIZE - 1) {
                    printf("String buffer overflow\n");
                    if (pe) exit(-1);
                    return;
                }
                s_buff[i++] = *cur_arg++;
            }
            s_buff[i] = 0x0;

            get_opcode(&embedded_op, s_buff, &embedded_tok_r);
            if (j == 0) {
                printf("Embedded operation cannot be first argument\n");
                if (pe) exit(-1);
                return;
            }
            if (embedded_op.opcode < ADD || MOD < embedded_op.opcode) {
                printf("Embedded operation must be an arithmetic instruction\n");
                if (pe) exit(-1);
                return;
            }
            parse_op(&embedded_op, &embedded_tok_r);
            if (!pe) eval_op(&embedded_op);
            args[j]->type = INT;
            args[j]->ptr.int_ptr = &i_buff;
        } else if ('0' <= *cur_arg && *cur_arg <= '9') {
            args[j]->type = INT;
            args[j]->transient = true;
            args[j]->ptr.int_ptr = simp_alloc(sizeof(long), INT);
            *args[j]->ptr.int_ptr = atoi(cur_arg);
        } else {
            printf("Unknown type for argument: %s\n", cur_arg);
            if (pe) exit(-1);
            return;
        }
    } while (++j < num_args);

    parse_no_args:

    if (!pe) return;

    memcpy(pe, op, sizeof(operation));
    pe++;
}
