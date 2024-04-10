#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_REGISTERS 4
#define GLOBAL_STACK_SIZE 1024

typedef enum {
    ASGN,
    ADD,
    SUBTR,
    MUL,
    DIV,
    PRINT,
    DONE,
} instructions_e;

typedef struct {
    instructions_e opcode;
    long *a1;
    long *a2;
    long *a3;
    char *lit;
} operation;

long g_registers[MAX_REGISTERS] = {};
long g_stack[GLOBAL_STACK_SIZE] = {};
long *sp = &(g_stack[0]);

long *parse_arg() {
    char *tmp = strtok(NULL, " \n");

    if (tmp[0] == 'r') {
        tmp = &(tmp[1]);
        int target_reg = atoi(tmp);
        if (target_reg >= MAX_REGISTERS) {
            printf("Invalid register number\n");
            exit(-1);
        }
        return &(g_registers[target_reg]);
    } else {
        *sp = atoi(tmp);
    }

    return sp++;
}

int main(int argc, char **argv) {
    long *bp = sp;
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

    if (memcmp(strtok(script, "\n"), "SIMPSCRIPT", 11) != 0) {
        printf("Not a SIMPSCRIPT file\n");
        return -1;
    }

    do {
        op.lit = strtok(NULL, " \n\t");
        if (memcmp(op.lit, "ASGN", 4) == 0) {
            op.opcode = ASGN;
        } else if (memcmp(op.lit, "ADD", 3) == 0) {
            op.opcode = ADD;
        } else if (memcmp(op.lit, "SUBTR", 5) == 0) {
            op.opcode = SUBTR;
        } else if (memcmp(op.lit, "MUL", 3) == 0) {
            op.opcode = MUL;
        } else if (memcmp(op.lit, "DIV", 3) == 0) {
            op.opcode = DIV;
        } else if (memcmp(op.lit, "PRINT", 5) == 0) {
            op.opcode = PRINT;
        } else if (memcmp(op.lit, "DONE", 4) == 0) {
            op.opcode = DONE;
        } else {
            op.opcode = -1;
        }

        switch (op.opcode) {
        case ASGN:
            op.a1 = parse_arg();
            op.a2 = parse_arg();

            *op.a1 = *op.a2;
            break;
        case ADD:
            op.a1 = parse_arg();
            op.a2 = parse_arg();
            op.a3 = parse_arg();

            *op.a1 = *op.a2 + *op.a3;
            break;
        case SUBTR:
            op.a1 = parse_arg();
            op.a2 = parse_arg();
            op.a3 = parse_arg();

            *op.a1 = *op.a2 - *op.a3;
            break;
        case MUL:
            op.a1 = parse_arg();
            op.a2 = parse_arg();
            op.a3 = parse_arg();

            *op.a1 = *op.a2 * *op.a3;
            break;
        case DIV:
            op.a1 = parse_arg();
            op.a2 = parse_arg();
            op.a3 = parse_arg();

            *op.a1 = *op.a2 / *op.a3;
            break;
        case PRINT:
            op.a1 = parse_arg();

            printf("%ld\n", *op.a1);
            break;
        case DONE:
            break;
        default:
            printf("Unknown command: %s\n", op.lit);
            return -1;
        }

        sp = bp;
    } while (op.opcode != DONE);

    return 0;
}
