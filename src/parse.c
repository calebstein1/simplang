#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"
#include "parse.h"

#define X(opcode, lit) lit,
char *opcode_lit[] = {
        OPCODE_TABLE
};
#undef X

void get_opcode(operation *op, char *tok) {
    int i = 0;
    char *lit = strtok(tok, " \n");
    int lit_len = strlen(lit);
    for (; memcmp(opcode_lit[i], lit, lit_len) != 0; i++) {
        if (i >= DONE) {
            op->opcode = -1;
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

void parse_op(operation *op) {
    switch (op->opcode) {
        case ASGN:
            parse_two_args(op);
            break;
        case RAND:
            parse_two_args(op);
            break;
        case LDSTR:
            parse_two_args(op);
            break;
        case GETI:
            parse_one_arg(op);
            break;
        case GETS:
            parse_one_arg(op);
            break;
        case ADD:
            parse_three_args(op);
            break;
        case SUBTR:
            parse_three_args(op);
            break;
        case MUL:
            parse_three_args(op);
            break;
        case DIV:
            parse_three_args(op);
            break;
        case INCR:
            parse_one_arg(op);
            break;
        case DECR:
            parse_one_arg(op);
            break;
        case SWP:
            parse_two_args(op);
            break;
        case BEGLP:
            break;
        case CONT:
            break;
        case ENDLPEQ:
            parse_two_args(op);
            break;
        case ENDLPNE:
            parse_two_args(op);
            break;
        case ENDLPLT:
            parse_two_args(op);
            break;
        case ENDLPLE:
            parse_two_args(op);
            break;
        case IFEQ:
            parse_two_args(op);
            break;
        case IFNE:
            parse_two_args(op);
            break;
        case IFLT:
            parse_two_args(op);
            break;
        case IFLE:
            parse_two_args(op);
            break;
        case ENDIF:
            break;
        case PRINT:
            parse_one_arg(op);
            break;
        case PRINTN:
            parse_one_arg(op);
            break;
        case PRINT_S:
            parse_one_arg(op);
            break;
        case PRINTN_S:
            parse_one_arg(op);
            break;
        case DONE:
            break;
    }
    if (!pe) return;

    memcpy(pe, op, sizeof(operation));
    pe++;
}
