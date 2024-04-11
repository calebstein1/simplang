#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#define MAX_REGISTERS 8
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_BUFF_SIZE 1024

typedef enum {
    ASGN,
    ADD,
    SUBTR,
    MUL,
    DIV,
    INCR,
    DECR,
    BEGLP,
    ENDLPEQ,
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

#endif //SIMPSCRIPT_DEFS_H
