#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "defs.h"
#include "args.h"
#include "opcode.h"

long g_registers[MAX_REGISTERS] = {};
long g_stack[GLOBAL_STACK_SIZE] = {};
char *j_stack[GLOBAL_STACK_SIZE] = {};
long *e_sp = &(g_stack[0]);
char **j_sp = &(j_stack[0]);

int main(int argc, char **argv) {
    long *e_bp = e_sp;
    char **j_bp = j_sp;
    operation op = {};
    struct stat script_stat = {};

    if (argc == 1) {
        printf("Need to specify a file\n");
        return -1;
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

    if (memcmp(strtok(script, "\n"), "SIMPLANG", 8) != 0) {
        printf("Not a SIMPLANG file\n");
        return -1;
    }

    do {
        op.lit = strtok(NULL, " \n\t");
        get_opcode(&op);

        switch (op.opcode) {
        case ASGN:
            parse_two_args(&op);
            *op.a1 = *op.a2;
            break;
        case ADD:
            parse_three_args(&op);
            *op.a1 = *op.a2 + *op.a3;
            break;
        case SUBTR:
            parse_three_args(&op);
            *op.a1 = *op.a2 - *op.a3;
            break;
        case MUL:
            parse_three_args(&op);
            *op.a1 = *op.a2 * *op.a3;
            break;
        case DIV:
            parse_three_args(&op);
            *op.a1 = *op.a2 / *op.a3;
            break;
        case INCR:
            parse_one_arg(&op);
            (*op.a1)++;
            break;
        case DECR:
            parse_one_arg(&op);
            (*op.a1)--;
            break;
        case BEGLP:
            *j_sp = op.lit;
            j_sp++;
            break;
        case ENDLPEQ:
            if (j_sp == j_bp) {
                break;
            }
            parse_two_args(&op);
            if (*op.a1 != *op.a2) {
                int i = 0;
                for (; i < 3; i++) {
                    while (*op.lit) {
                        op.lit++;
                    }
                    *op.lit = ' ';
                }
                for (; op.lit >= *(j_sp - 1); op.lit--) {
                    if (!*op.lit) {
                        *op.lit = ' ';
                    }
                }
                strtok(++op.lit, " \n\t");
            } else {
                j_sp--;
            }
            break;
        case PRINT:
            parse_one_arg(&op);
            printf("%ld\n", *op.a1);
            break;
        case DONE:
            break;
        default:
            printf("Unknown command: %s\n", op.lit);
            return -1;
        }

        e_sp = e_bp;
    } while (op.opcode != DONE);

    return 0;
}
