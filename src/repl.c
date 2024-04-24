#include <stdio.h>

#include "defs.h"
#include "parse.h"
#include "eval.h"
#include "repl.h"

void launch_repl() {
    printf("Welcome to Simplang! Enter your commands at the prompt (done to exit):\n");

    while(1) {
        operation op = { .target = { -1, -1, -1 } };
        char *tok_pos = NULL;
        printf(">> ");
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        if (s_buff[0] == '\n') continue;
        get_opcode(&op, s_buff, &tok_pos);
        if (op.opcode == DONE) break;
        if (!op.opcode || op.opcode == CMNT) continue;
        if (BEGLP <= op.opcode && op.opcode <= ENDLPGE) {
            printf("Looping not yet supported in repl mode\n");
            continue;
        } else if (IFEQ <= op.opcode && op.opcode <= ENDIF) {
            printf("Conditional statements not yet supported in repl mode\n");
            continue;
        }
        parse_op(&op, &tok_pos);
        eval_op(&op);
    }
}
