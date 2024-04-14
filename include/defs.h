#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#define MAX_PROGRAM_SIZE 2048
#define MAX_REGISTERS 8
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_HEAP_SIZE 4096
#define GLOBAL_BUFF_SIZE 512

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
X(CMNT, ";")           \
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
} operation;

extern long g_registers[MAX_REGISTERS];
extern char *s_registers[MAX_REGISTERS];
extern char g_heap[GLOBAL_HEAP_SIZE];
extern char s_buff[GLOBAL_BUFF_SIZE];
extern long *e_sp, *e_bp;
extern operation **j_sp, **j_bp;
extern operation *pp, *pe;

#endif //SIMPSCRIPT_DEFS_H
