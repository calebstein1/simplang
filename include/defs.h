#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#define MAX_REGISTERS 8
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_BUFF_SIZE 1024

typedef enum {
    ASGN,
    LDSTR,
    ADD,
    SUBTR,
    MUL,
    DIV,
    INCR,
    DECR,
    BEGLP,
    ENDLPEQ,
    ENDLPNE,
    ENDLPLT,
    ENDLPLE,
    IFEQ,
    IFNE,
    IFLT,
    IFLE,
    ENDIF,
    PRINT_S,
    PRINT,
    DONE,
} instructions_e;

typedef struct {
    instructions_e opcode;
    void **a1;
    void **a2;
    void **a3;
    char *lit;
} operation;

#endif //SIMPSCRIPT_DEFS_H
