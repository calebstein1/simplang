#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "parse.h"
#include "eval.h"
#include "repl.h"
#include "malloc.h"

void launch_repl() {
    operation *hist = NULL;
    int pp_idx = 0, hist_bins = 1;
    malloc_or_fail(hist, HIST_BIN_SIZE * hist_bins, sizeof(operation));
    pp = hist;

    printf("Welcome to Simplang! Enter your commands at the prompt (done to exit):\n");

    for(;;) {
        int i = 0;
        operation op = {};
        for (; i < MAX_ARGUMENTS; i++) {
            op.target[i] = -1;
        }
        char *tok_pos = NULL;
        printf("[%.3d] >> ", pp_idx);
        fgets(s_buff, GLOBAL_BUFF_SIZE, stdin);
        if (s_buff[0] == '\n') continue;
        get_opcode(&op, s_buff, &tok_pos);
        if (op.opcode == DONE) {
            free(hist);
            break;
        }
        if (!op.opcode || op.opcode == CMNT) continue;
        if (BEGLP <= op.opcode && op.opcode <= ENDLPGE) {
            printf("Looping not yet supported in repl mode\n");
            continue;
        } else if (IFEQ <= op.opcode && op.opcode <= ENDIF) {
            printf("Conditional statements not yet supported in repl mode\n");
            continue;
        }
        parse_op(&op, &tok_pos);
        memcpy(pp, &op, sizeof(op));
        if (++pp_idx >= 8 * hist_bins) {
            realloc_or_fail(hist, HIST_BIN_SIZE * ++hist_bins, sizeof(operation));
            pp = hist + (pp_idx - 1);
        }
        eval_op(pp++);
    }
}
