#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "heap.h"

char *opcode_lit[] = {
    #define X(opcode, lit, ...) lit,
    OPCODE_TABLE
    #undef X
};

void get_opcode(operation *op, char *tok) {
    int i = 1, l = 0;
    char *lit = strtok(tok, " \n");
    int lit_len = strlen(lit);
    for (; memcmp(opcode_lit[i], lit, (l = strlen(opcode_lit[i])) > lit_len ? l : lit_len) != 0; i++) {
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
    char *tmp, *str;
    tmp = str = NULL;
    int i, j, num_args;
    j = num_args = 0;
    dyn_ptr_t *args[] = {
            &op->a1,
            &op->a2,
            &op->a3,
    };

    static void *parse_fn_tbl[] = {
        #define X(opcode, lit, parse_fn) parse_fn,
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

    do {
        tmp = strtok(NULL, " \t");
        if (*tmp == 'r' || *tmp == 's') {
            int target_reg = atoi(tmp + 1);
            if (target_reg >= MAX_REGISTERS) {
                printf("Invalid register number\n");
                if (pe) exit(-1);
                op->opcode = INVLD;
                return;
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
        } else {
            if (*tmp == '"') {
                i = 0;
                tmp++;
                while (*tmp != '"') {
                    if (!*tmp) {
                        *tmp = ' ';
                    }
                    if (i >= GLOBAL_BUFF_SIZE - 1) {
                        printf("String buffer overflow\n");
                        if (pe) exit(-1);
                        return;
                    }
                    s_buff[i++] = *tmp++;
                }
                s_buff[i] = 0x0;
                str = simp_alloc(i, STR);
                strcpy(str, s_buff);
                strtok(tmp, " \t");

                args[j]->type = CHAR;
                args[j]->ptr.char_ptr = str;
            } else {
                *e_sp = atoi(tmp);
                if (e_sp + 1 > e_bp + GLOBAL_STACK_SIZE) {
                    printf("Eval stack overflow\n");
                    if (pe) exit(-1);
                    e_sp = e_bp;
                    op->opcode = INVLD;
                    return;
                }

                args[j]->type = INT;
                args[j]->ptr.int_ptr = e_sp++;
            }
        }
    } while (++j < num_args);

    parse_no_args:

    if (!pe) return;

    memcpy(pe, op, sizeof(operation));
    pe++;
}
