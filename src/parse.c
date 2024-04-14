#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"
#include "parse.h"

#define X(opcode, lit, parse_fn) lit,
char *opcode_lit[] = {
        OPCODE_TABLE
};
#undef X

int greater_of(int a, int b) {
    return a > b ? a : b;
}

void get_opcode(operation *op, char *tok) {
    int i = 0;
    char *lit = strtok(tok, " \n");
    int lit_len = strlen(lit);
    for (; memcmp(opcode_lit[i], lit, greater_of(lit_len, strlen(opcode_lit[i]))) != 0; i++) {
        if (i >= DONE) {
            op->opcode = -1;
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

void *parse_arg() {
    bool str_lit = false;
    int i = 0, j;
    char *tmp = strtok(NULL, " \t");

    if (*tmp == 'r' || *tmp == 's') {
        int target_reg = atoi(tmp + 1);
        if (target_reg >= MAX_REGISTERS) {
            printf("Invalid register number\n");
            exit(-1);
        }

        return *tmp == 'r' ? &(g_registers[target_reg]) : &(s_registers[target_reg]);
    } else {
        if (*tmp == '"') {
            tmp++;
            str_lit = true;
            while (g_heap[i]) {
                i++;
                if(!g_heap[i] && g_heap[i + 1]) i++;
            }
            if (i > 0) i++;
            j = i;
            while (*tmp != '"') {
                if (!*tmp) {
                    *tmp = ' ';
                }
                if (i >= GLOBAL_BUFF_SIZE - 1) {
                    printf("String buffer overflow\n");
                    exit(-1);
                }
                g_heap[i++] = *tmp++;
            }
            g_heap[i] = 0x0;
            strtok(tmp, " \t");
        } else {
            *e_sp = atoi(tmp);
            if (e_sp + 1 > e_bp + GLOBAL_STACK_SIZE) {
                printf("Eval stack overflow\n");
                exit(-1);
            }
        }
    }

    return str_lit ? &(g_heap[j]) : e_sp++;
}

void parse_no_args(operation *op) {}

void parse_one_arg(operation *op) {
    op->a1 = parse_arg();
}

void parse_two_args(operation *op) {
    op->a1 = parse_arg();
    op->a2 = parse_arg();
}

void parse_three_args(operation *op) {
    op->a1 = parse_arg();
    op->a2 = parse_arg();
    op->a3 = parse_arg();
}

#define X(opcode, lit, parse_fn) parse_fn,
void (*parse_fn[])(operation *op) = {
        OPCODE_TABLE
};
#undef X

void parse_op(operation *op) {
    if (op->opcode == -1) {
        printf("%s\n", s_buff);
        if (pe) exit(-1);

        return;
    }
    parse_fn[op->opcode](op);
    if (!pe) return;

    memcpy(pe, op, sizeof(operation));
    pe++;
}
