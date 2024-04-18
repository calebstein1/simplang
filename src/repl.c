#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "eval.h"
#include "repl.h"

void launch_repl(operation *op) {
    printf("Welcome to Simplang! Enter your commands at the prompt (done to exit):\n");

    while(op->opcode != DONE) {
        printf(">> ");
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        if (s_buff[0] == '\n') continue;
        get_opcode(op, s_buff);
        if (op->opcode == CMNT) continue;
        if (op->opcode == BEGLP || (ENDLPEQ <= op->opcode && op->opcode <= ENDLPGE)) {
            printf("Looping not yet supported in repl mode\n");
            continue;
        } else if (op->opcode == ENDIF || (IFEQ <= op->opcode && op->opcode <= IFGE)) {
            printf("Conditional statements not yet supported in repl mode\n");
            continue;
        }
        parse_op(op);
        eval_op(op);
    }
}