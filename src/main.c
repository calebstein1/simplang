#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "defs.h"
#include "eval.h"
#include "opcode.h"
#include "repl.h"

long g_registers[MAX_REGISTERS] = {};
char *s_registers[MAX_REGISTERS] = {};
char s_buff[GLOBAL_BUFF_SIZE] = {};
long g_stack[GLOBAL_STACK_SIZE] = {};
char *j_stack[GLOBAL_STACK_SIZE] = {};
long *e_sp, *e_bp;
char **j_sp, **j_bp;

int main(int argc, char **argv) {
    srand(time(NULL));
    e_sp = e_bp = &(g_stack[0]);
    j_sp = j_bp = &(j_stack[0]);
    operation op = {};
    struct stat script_stat = {};
    bool start_adv = false;

    if (argc == 1) {
        launch_repl(&op);
        return 0;
    } else if (argc > 2) {
        printf("Please specify exactly one file\n");
        return -1;
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

    lseek(fd, 0, SEEK_SET);
    read (fd, script, script_stat.st_size);
    close(fd);

    if (memcmp(script, "#!", 2) == 0) {
        start_adv = true;
        strtok(script, "\n");
    }

    do {
        op.lit = start_adv ? strtok(NULL, " \n\t") : strtok(script, " \n\t");
        start_adv = true;
        get_opcode(&op);
        eval_op(&op);
    } while (op.opcode != DONE);

    return 0;
}
