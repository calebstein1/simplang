#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "eval.h"
#include "opcode.h"
#include "repl.h"

void launch_repl(operation *op) {
    char *cmd = NULL;
    size_t len = 0;
    printf("Welcome to Simplang! Enter your commands at the prompt (done to exit):\n");

    while(op->opcode != DONE) {
        printf(">> ");
        getline(&cmd, &len, stdin);
        get_opcode(op, cmd);
        if (op->opcode == BEGLP || op->opcode == ENDLPEQ) {
            printf("Looping not supported in repl mode\n");
            continue;
        }
        parse_op(op);
        eval_op(op);
    }

    free(cmd);
}