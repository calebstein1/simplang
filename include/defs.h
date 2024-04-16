#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#define MAX_PROGRAM_SIZE 2048
#define MAX_REGISTERS 8
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_HEAP_SIZE 4096
#define GLOBAL_BUFF_SIZE 512

#define OPCODE_TABLE                                 \
X(ASGN, "asgn", &parse_two_args, &&ASGN)             \
X(RAND, "rand", &parse_two_args, &&RAND)             \
X(LDSTR, "ldstr", &parse_two_args, &&LDSTR)          \
X(GETI, "geti", &parse_one_arg, &&GETI)              \
X(GETS, "gets", &parse_one_arg, &&GETS)              \
X(ADD, "add", &parse_three_args, &&ADD)              \
X(SUBTR, "subtr", &parse_three_args, &&SUBTR)        \
X(MUL, "mul", &parse_three_args, &&MUL)              \
X(DIV, "div", &parse_three_args, &&DIV)              \
X(MOD, "mod", &parse_three_args, &&MOD)              \
X(INCR, "incr", &parse_one_arg, &&INCR)              \
X(DECR, "decr", &parse_one_arg, &&DECR)              \
X(SWP, "swp", &parse_two_args, &&SWP)                \
X(BEGLP, "beglp", &parse_no_args, &&BEGLP)           \
X(CONT, "cont", &parse_no_args, &&CONT)              \
X(ENDLPEQ, "endlpeq", &parse_two_args, &&ENDLPEQ)    \
X(ENDLPNE, "endlpne", &parse_two_args, &&ENDLPNE)    \
X(ENDLPLT, "endlplt", &parse_two_args, &&ENDLPLT)    \
X(ENDLPLE, "endlple", &parse_two_args, &&ENDLPLE)    \
X(IFEQ, "ifeq", &parse_two_args, &&IFEQ)             \
X(IFNE, "ifne", &parse_two_args, &&IFNE)             \
X(IFLT, "iflt", &parse_two_args, &&IFLT)             \
X(IFLE, "ifle", &parse_two_args, &&IFLE)             \
X(ENDIF, "endif", &parse_no_args, &&NOP)             \
X(PRINT, "print", &parse_one_arg, &&PRINT)           \
X(PRINTN, "printn", &parse_one_arg, &&PRINTN)        \
X(CMNT, ";", &parse_no_args, &&NOP)                  \
X(DONE, "done", &parse_no_args, &&NOP)               \
X(NOP, "", &parse_no_args, &&NOP)

#define PTR_TYPE_TABLE               \
X(NONE, &&NOP, &&NOP)                \
X(CHAR, &&PRINT_CHAR, &&PRINTN_CHAR) \
X(STR, &&PRINT_STR, &&PRINTN_STR)    \
X(INT, &&PRINT_INT, &&PRINTN_INT)

typedef enum {
    #define X(opcode, lit, parse_fn, eval_lbl) opcode,
    OPCODE_TABLE
    #undef X
} instructions_e;

typedef union {
    char *char_ptr;
    char **str_ptr;
    long *int_ptr;
    void *void_ptr;
} op_ptr_u;

typedef enum {
    #define X(type, print_lbl, printn_lbl) type,
    PTR_TYPE_TABLE
    #undef X
} ptr_type_e;

typedef struct {
    op_ptr_u ptr;
    ptr_type_e type;
} op_ptr_t;

typedef struct {
    instructions_e opcode;
    op_ptr_t a1;
    op_ptr_t a2;
    op_ptr_t a3;
} operation;

extern long g_registers[MAX_REGISTERS];
extern char *s_registers[MAX_REGISTERS];
extern char g_heap[GLOBAL_HEAP_SIZE];
extern char s_buff[GLOBAL_BUFF_SIZE];
extern long *e_sp, *e_bp;
extern operation **j_sp, **j_bp;
extern operation *pp, *pe;

#endif //SIMPSCRIPT_DEFS_H
