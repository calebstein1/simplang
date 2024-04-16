#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "heap.h"

char *opcode_lit[] = {
    #define X(opcode, lit, parse_fn, eval_lbl) lit,
    OPCODE_TABLE
    #undef X
};

int greater_of(int a, int b) {
    return a > b ? a : b;
}

void get_opcode(operation *op, char *tok) {
    int i = 1;
    char *lit = strtok(tok, " \n");
    int lit_len = strlen(lit);
    for (; memcmp(opcode_lit[i], lit, greater_of(lit_len, strlen(opcode_lit[i]))) != 0; i++) {
        if (i >= DONE) {
            op->opcode = INVLD;
            char err_msg[lit_len + 24];
            sprintf(err_msg, "Invalid instruction: %s", lit);
            if (strlen(err_msg) > GLOBAL_BUFF_SIZE){
                sprintf(err_msg, "Invalid instruction: string buffer overflow");
            }
            strcpy(s_buff, err_msg);

            return;
        }
    }
    op->opcode = i;
}

void parse_op(operation *op) {
    bool str_lit = false;
    char *tmp, *str;
    tmp = str = NULL;
    int i, j, num_args;
    i = j = num_args = 0;
    op_ptr_t *args[] = {
            &op->a1,
            &op->a2,
            &op->a3,
    };

    static void *parse_fn_tbl[] = {
    #define X(opcode, lit, parse_fn, eval_lbl) parse_fn,
        OPCODE_TABLE
    #undef X
    };

    if (!op->opcode) {
        printf("%s\n", s_buff);
        if (pe) exit(-1);

        return;
    }

    goto *parse_fn_tbl[op->opcode];

    parse_three_args:
        ++num_args;
    parse_two_args:
        ++num_args;
    parse_one_arg:
        ++num_args;

    while (num_args-- > 0) {
        tmp = strtok(NULL, " \t");
        if (*tmp == 'r' || *tmp == 's') {
            int target_reg = atoi(tmp + 1);
            if (target_reg >= MAX_REGISTERS) {
                printf("Invalid register number\n");
                exit(-1);
            }

            switch (*tmp) {
                case 'r':
                    args[j]->type = INT;
                    args[j]->ptr.int_ptr = &(g_registers[target_reg]);
                    break;
                case 's':
                    args[j]->type = STR;
                    args[j]->ptr.str_ptr = &(s_registers[target_reg]);
                    break;
            }

            j++;
            continue;
        } else {
            if (*tmp == '"') {
                tmp++;
                str_lit = true;
                while (*tmp != '"') {
                    if (!*tmp) {
                        *tmp = ' ';
                    }
                    if (i >= GLOBAL_BUFF_SIZE - 1) {
                        printf("String buffer overflow\n");
                        exit(-1);
                    }
                    s_buff[i++] = *tmp++;
                }
                s_buff[i] = 0x0;
                str = simp_alloc(i, STR);
                strcpy(str, s_buff);
                strtok(tmp, " \t");
            } else {
                *e_sp = atoi(tmp);
                if (e_sp + 1 > e_bp + GLOBAL_STACK_SIZE) {
                    printf("Eval stack overflow\n");
                    exit(-1);
                }
            }
        }

        if (str_lit) {
            args[j]->type = CHAR;
            args[j]->ptr.char_ptr = str;
        } else {
            args[j]->type = INT;
            args[j]->ptr.int_ptr = e_sp++;
        }

        j++;
    }

    parse_no_args:

    if (!pe) return;

    memcpy(pe, op, sizeof(operation));
    pe++;
}