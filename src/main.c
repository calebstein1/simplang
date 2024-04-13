#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "defs.h"
#include "parse.h"
#include "eval.h"
#include "opcode.h"
#include "repl.h"

long g_registers[MAX_REGISTERS] = {};
char *s_registers[MAX_REGISTERS] = {};
char s_buff[GLOBAL_BUFF_SIZE] = {};
long g_stack[GLOBAL_STACK_SIZE] = {};
char g_heap[GLOBAL_HEAP_SIZE] = {};
operation *j_stack[GLOBAL_STACK_SIZE] = {};
long *e_sp, *e_bp;
operation **j_sp, **j_bp;
operation *pp, *pe;

int main(int argc, char **argv) {
    srand(time(NULL));
    e_sp = e_bp = &(g_stack[0]);
    j_sp = j_bp = j_stack;
    char *cur_tok = NULL;
    char *tok_r = NULL;
    struct stat script_stat = {};
    bool start_adv = false;

    if (argc == 1) {
        operation op = {};
        pp = pe = NULL;
        launch_repl(&op);
        return 0;
    } else if (argc > MAX_REGISTERS + 2) {
        printf("Too many arguments\n");
        return -1;
    }

    int i = 2;
    for (; i < argc; i++) {
        g_registers[i - 2] = atoi(argv[i]);
    }
    
    int fd;

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }

    if (fstat(fd, &script_stat) != 0) {
        perror("fstat");
        return -1;
    }
    char script[script_stat.st_size];
    operation program[MAX_PROGRAM_SIZE] = {};
    pp = pe = program;

    lseek(fd, 0, SEEK_SET);
    read (fd, script, script_stat.st_size);
    close(fd);

    if (memcmp(script, "#!", 2) == 0) {
        start_adv = true;
        strtok_r(script, "\n", &tok_r);
    }

    do {
        operation op = {};
        cur_tok = start_adv ? strtok_r(NULL, "\n", &tok_r) : strtok_r(script, "\n", &tok_r);
        start_adv = true;
        get_opcode(&op, cur_tok);
        if (op.opcode == CMNT) continue;
        parse_op(&op);
    } while ((pe - 1)->opcode != DONE);

    do {
        eval_op(pp);
    } while (++pp < pe);

    return 0;
}
