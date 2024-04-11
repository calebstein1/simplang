#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "args.h"
#include "globals.h"

long *parse_arg() {
    char *tmp = strtok(NULL, " \n\t");

    if (tmp[0] == 'r') {
        tmp = &(tmp[1]);
        int target_reg = atoi(tmp);
        if (target_reg >= MAX_REGISTERS) {
            printf("Invalid register number\n");
            exit(-1);
        }
        return &(g_registers[target_reg]);
    } else {
        *e_sp = atoi(tmp);
    }
    if (e_sp + 1 > e_bp + GLOBAL_STACK_SIZE) {
        printf("Eval stack overflow\n");
        exit(-1);
    }

    return e_sp++;
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
