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
    int i;
    op->arg_count = 0;

    while ((cur_arg = strtok_r(NULL, " \t", tok_pos))) {
        int arg_len = strlen(cur_arg);
        if (cur_arg[arg_len - 1] == '\n') {
            cur_arg[--arg_len] = 0x0;
        }
        if (!arg_len) break;
        if (memcmp(cur_arg, "in", arg_len > 2 ? arg_len : 2) == 0) {}
        else if (memcmp(cur_arg, "next", arg_len > 4 ? arg_len : 4) == 0) {
            op->target[op->arg_count] = MAX_REGISTERS;
        } else if (('a' <= *cur_arg && *cur_arg <= 'z') || ('A' <= *cur_arg && *cur_arg <= 'Z')) {
            int l;
            i = l = 0;

            if (cur_arg[arg_len - 1] == ']') {
                cur_arg[--arg_len] = 0x0;
                while (cur_arg[--arg_len] != '[') {}
                cur_arg[arg_len] = 0x0;
                op->arg_list[op->arg_count].idx = atoi(cur_arg + arg_len + 1);
            }

            for (; i < MAX_REGISTERS; i++) {
                if (!reg_lbls[i]) {
                    reg_lbls[i] = simp_alloc(arg_len, STR);
                    strcpy(reg_lbls[i], cur_arg);
                    op->target[op->arg_count] = i;
                    break;
                }
                if (memcmp(cur_arg, reg_lbls[i], (l = strlen(reg_lbls[i])) > arg_len ? l : arg_len) == 0) {
                    op->target[op->arg_count] = i;
                    break;
                }
            }
            if (i == MAX_REGISTERS) {
                printf("Too many variables assigned\n");
                if (pe) exit(-1);
            }
        } else if (*cur_arg == '"') {
            bool has_spaces = false;
            i = 0;
            cur_arg++;
            while (*cur_arg != '"') {
                if (!*cur_arg) {
                    *cur_arg = ' ';
                    has_spaces = true;
                }
                if (i >= GLOBAL_BUFF_SIZE - 1) {
                    printf("String buffer overflow\n");
                    if (pe) exit(-1);
                    return;
                }
                s_buff[i++] = *cur_arg++;
            }
            s_buff[i] = 0x0;
            op->arg_list[op->arg_count].type = STR;
            op->arg_list[op->arg_count].transient = true;
            op->arg_list[op->arg_count].ptr.str_ptr = simp_alloc(i, STR);
            strcpy(op->arg_list[op->arg_count].ptr.str_ptr, s_buff);
            if (has_spaces) strtok_r(NULL, "\"", tok_pos);
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
            if (op->arg_count == 0) {
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
            op->arg_list[op->arg_count].type = INT;
            op->arg_list[op->arg_count].ptr.int_ptr = &i_buff;
        } else if (('0' <= *cur_arg && *cur_arg <= '9') || *cur_arg == '-') {
            op->arg_list[op->arg_count].type = INT;
            op->arg_list[op->arg_count].transient = true;
            op->arg_list[op->arg_count].ptr.int_ptr = simp_alloc(sizeof(long), INT);
            *op->arg_list[op->arg_count].ptr.int_ptr = atoi(cur_arg);
        } else {
            printf("Unknown type for argument: %s\n", cur_arg);
            if (pe) exit(-1);
            return;
        }
        ++op->arg_count;
    }

    if (!pe) return;

    memcpy(pe++, op, sizeof(operation));
}
