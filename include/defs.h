#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#define MAX_REGISTERS 8
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_BUFF_SIZE 1024

#define OPCODE_TABLE   \
X(ASGN, "asgn")        \
X(RAND, "rand")        \
X(LDSTR, "ldstr")      \
X(GETI, "geti")        \
X(GETS, "gets")        \
X(ADD, "add")          \
X(SUBTR, "subtr")      \
X(MUL, "mul")          \
X(DIV, "div")          \
X(INCR, "incr")        \
X(DECR, "decr")        \
X(SWP, "swp")          \
X(BEGLP, "beglp")      \
X(CONT, "cont")        \
X(ENDLPEQ, "endlpeq")  \
X(ENDLPNE, "endlpne")  \
X(ENDLPLT, "endlplt")  \
X(ENDLPLE, "endlple")  \
X(IFEQ, "ifeq")        \
X(IFNE, "ifne")        \
X(IFLT, "iflt")        \
X(IFLE, "ifle")        \
X(ENDIF, "endif")      \
X(PRINT, "print")      \
X(PRINTN, "printn")    \
X(PRINT_S, "print_s")  \
X(PRINTN_S, "printn_s")\
X(DONE, "done")

typedef enum {
    #define X(opcode, lit) opcode,
    OPCODE_TABLE
    #undef X
} instructions_e;

typedef struct {
    instructions_e opcode;
    void **a1;
    void **a2;
    void **a3;
    char *lit;
} operation;

#endif //SIMPSCRIPT_DEFS_H
