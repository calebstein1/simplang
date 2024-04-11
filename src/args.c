#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"
#include "args.h"
#include "globals.h"

void *parse_arg() {
    bool str_lit = false;
    char *tmp = strtok(NULL, " \n\t");

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
            int i = 0;
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
            strtok(tmp, " \n\t");
        } else {
            *e_sp = atoi(tmp);
            if (e_sp + 1 > e_bp + GLOBAL_STACK_SIZE) {
                printf("Eval stack overflow\n");
                exit(-1);
            }
        }
    }

    return str_lit ? s_buff : e_sp++;
}

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
