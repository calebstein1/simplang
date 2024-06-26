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
#include "repl.h"
#include "heap.h"

dyn_ptr_t g_registers[MAX_REGISTERS] = {};
dyn_ptr_t loop_next = {};
long i_buff;
char s_buff[GLOBAL_BUFF_SIZE] = {};
operation *j_stack[GLOBAL_STACK_SIZE] = {};
operation **j_sp, **j_bp;
operation *pp, *pe;
bool repl = false;
int repl_l_offset = 0;

int main(int argc, char **argv) {
    srand(time(NULL));
    init_heap();
    j_sp = j_bp = j_stack;
    char *cur_tok = NULL;
    char *tok_r = NULL;
    struct stat script_stat = {};
    bool start_adv = false;

    if (argc == 1) {
        pp = pe = NULL;
        launch_repl();
        return 0;
    } else if (argc > MAX_REGISTERS + 2) {
        printf("Too many arguments\n");
        return -1;
    }

    int i = 2;
    for (; i < argc; i++) {
        g_registers[i - 2].type = INT;
        g_registers[i - 2].ptr = simp_alloc(sizeof(long), INT);
        *(long *)g_registers[i - 2].ptr = atoi(argv[i]);
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
    char script[script_stat.st_size + 1];
    operation program[MAX_PROGRAM_SIZE] = {};
    pp = pe = program;

    lseek(fd, 0, SEEK_SET);
    read (fd, script, script_stat.st_size);
    close(fd);
    script[script_stat.st_size] = 0x0;

    if (strncmp(script, "#!", 2) == 0) {
        start_adv = true;
        strtok_r(script, "\n", &tok_r);
    }

    do {
        operation op = {};
        for (i = 0; i < MAX_ARGUMENTS; i++) {
            op.target[i] = -1;
        }
        char *tok_pos = NULL;
        cur_tok = start_adv ? strtok_r(NULL, "\n", &tok_r) : strtok_r(script, "\n", &tok_r);
        start_adv = true;
        get_opcode(&op, cur_tok, &tok_pos);
        if (op.opcode == CMNT) continue;
        parse_op(&op, &tok_pos);
    } while ((pe - 1)->opcode != DONE);

    do {
        eval_op(pp);
    } while (++pp < pe);

    return 0;
}
