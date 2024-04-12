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
operation *j_stack[GLOBAL_STACK_SIZE] = {};
long *e_sp, *e_bp;
operation **j_sp, **j_bp;
operation *pp, *pe;

int main(int argc, char **argv) {
    srand(time(NULL));
    e_sp = e_bp = &(g_stack[0]);
    j_sp = j_bp = j_stack;
    operation op = {};
    char *cur_tok = NULL;
    struct stat script_stat = {};
    bool start_adv = false;

    if (argc == 1) {
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
        strtok(script, "\n");
    }

    do {
        cur_tok = start_adv ? strtok(cur_tok, "\n") : strtok(script, "\n");
        start_adv = true;
        printf("Current pe: %p\n", pe);
        printf("Current token: %s\n", cur_tok);
        get_opcode(&op, cur_tok);
        printf("Parsing %s...\n", cur_tok);
        parse_op(&op);
        printf("Parsed\n");
    } while (op.opcode != DONE);

    do {
        printf("Evaluating...\n");
        eval_op(pp);
        printf("Evaluated\n");
    } while (++pp < pe);

    return 0;
}
