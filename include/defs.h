#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#define MAX_PROGRAM_SIZE 2048
#define MAX_REGISTERS 8
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_HEAP_SIZE 4096
#define GLOBAL_BUFF_SIZE 512

#define OPCODE_TABLE                       \
X(INVLD, "", &&parse_no_args)              \
X(ASGN, "asgn", &&parse_two_args)          \
X(RAND, "rand", &&parse_two_args)          \
X(LDSTR, "ldstr", &&parse_two_args)        \
X(GETOPT, "getopt", &&parse_two_args)      \
X(GETI, "geti", &&parse_one_arg)           \
X(GETS, "gets", &&parse_one_arg)           \
X(ADD, "add", &&parse_two_args)            \
X(SUBTR, "subtr", &&parse_two_args)        \
X(MUL, "mul", &&parse_two_args)            \
X(DIV, "div", &&parse_two_args)            \
X(MOD, "mod", &&parse_two_args)            \
X(INCR, "incr", &&parse_one_arg)           \
X(DECR, "decr", &&parse_one_arg)           \
X(SWP, "swp", &&parse_two_args)            \
X(BEGLP, "beglp", &&parse_no_args)         \
X(CONT, "cont", &&parse_no_args)           \
X(ENDLPEQ, "endlpeq", &&parse_two_args)    \
X(ENDLPNE, "endlpne", &&parse_two_args)    \
X(ENDLPLT, "endlplt", &&parse_two_args)    \
X(ENDLPLE, "endlple", &&parse_two_args)    \
X(ENDLPGT, "endlpgt", &&parse_two_args)    \
X(ENDLPGE, "endlpge", &&parse_two_args)    \
X(IFEQ, "ifeq", &&parse_two_args)          \
X(IFNE, "ifne", &&parse_two_args)          \
X(IFLT, "iflt", &&parse_two_args)          \
X(IFLE, "ifle", &&parse_two_args)          \
X(IFGT, "ifgt", &&parse_two_args)          \
X(IFGE, "ifge", &&parse_two_args)          \
X(ENDIF, "endif", &&parse_no_args)         \
X(PRINT, "print", &&parse_one_arg)         \
X(PRINTN, "printn", &&parse_one_arg)       \
X(CMNT, ";", &&parse_no_args)              \
X(DONE, "done", &&parse_no_args)

#define PTR_TYPE_TABLE \
X(NONE)                \
X(CHAR)                \
X(STR)                 \
X(INT)

typedef enum {
    #define X(opcode, ...) opcode,
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
    #define X(type, ...) type,
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
extern char s_buff[GLOBAL_BUFF_SIZE];
extern long *e_sp, *e_bp;
extern operation **j_sp, **j_bp;
extern operation *pp, *pe;

#endif //SIMPSCRIPT_DEFS_H
