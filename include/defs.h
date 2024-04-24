#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#include <stdbool.h>

#define MAX_PROGRAM_SIZE 2048
#define MAX_REGISTERS 24
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_HEAP_SIZE 4096
#define GLOBAL_BUFF_SIZE 512

#define OPCODE_TABLE                                         \
X(INVLD, "", &&parse_no_args)                                \
X(ASGN, "assign", &&parse_two_args)                          \
X(NEWARR, "new_array", &&parse_two_args)                     \
X(RAND, "random", &&parse_two_args)                          \
X(GETOPT, "get_option", &&parse_two_args)                    \
X(GET, "get_input", &&parse_one_arg)                         \
X(ADD, "add", &&parse_two_args)                              \
X(SUBTR, "subtract", &&parse_two_args)                       \
X(MUL, "multiply", &&parse_two_args)                         \
X(DIV, "divide", &&parse_two_args)                           \
X(MOD, "modulo", &&parse_two_args)                           \
X(INCR, "increment", &&parse_one_arg)                        \
X(DECR, "decrement", &&parse_one_arg)                        \
X(SWP, "swap", &&parse_two_args)                             \
X(BEGLP, "begin_loop", &&parse_no_args)                      \
X(FOREACH, "for_each", &&parse_two_args)                     \
X(CONT, "continue", &&parse_no_args)                         \
X(ENDLP, "end_loop", &&parse_no_args)                        \
X(ENDLPEQ, "end_loop_if_equal", &&parse_two_args)            \
X(ENDLPNE, "end_loop_if_not_equal", &&parse_two_args)        \
X(ENDLPLT, "end_loop_if_less", &&parse_two_args)             \
X(ENDLPLE, "end_loop_if_less_or_equal", &&parse_two_args)    \
X(ENDLPGT, "end_loop_if_greater", &&parse_two_args)          \
X(ENDLPGE, "end_loop_if_greater_or_equal", &&parse_two_args) \
X(IFEQ, "if_equal", &&parse_two_args)                        \
X(IFNE, "if_not_equal", &&parse_two_args)                    \
X(IFLT, "if_less", &&parse_two_args)                         \
X(IFLE, "if_less_or_equal", &&parse_two_args)                \
X(IFGT, "if_greater", &&parse_two_args)                      \
X(IFGE, "if_greater_or_equal", &&parse_two_args)             \
X(ENDIF, "end_if", &&parse_no_args)                          \
X(PRINT, "print", &&parse_one_arg)                           \
X(PRINTN, "print_n", &&parse_one_arg)                        \
X(CMNT, ";", &&parse_no_args)                                \
X(DONE, "done", &&parse_no_args)

#define PTR_TYPE_TABLE          \
X(NONE, &&INVLD)                \
X(STR, &&LDSTR)                 \
X(INT, &&LDINT)                 \
X(ARR, &&LDINT)

typedef enum {
    #define X(opcode, ...) opcode,
    OPCODE_TABLE
    #undef X
} instructions_e;

typedef union {
    char *str_ptr;
    long *int_ptr;
    void *void_ptr;
} dyn_ptr_u;

typedef enum {
    #define X(type, ...) type,
    PTR_TYPE_TABLE
    #undef X
} ptr_type_e;

typedef struct {
    dyn_ptr_u ptr;
    ptr_type_e type;
    int arr_size;
    int idx;
    bool transient;
} dyn_ptr_t;

typedef struct {
    instructions_e opcode;
    int target[3];
    dyn_ptr_t a1;
    dyn_ptr_t a2;
    dyn_ptr_t a3;
    bool embedded;
} operation;

extern dyn_ptr_t g_registers[MAX_REGISTERS];
extern dyn_ptr_t loop_next;
extern long i_buff;
extern char s_buff[GLOBAL_BUFF_SIZE];
extern operation **j_sp, **j_bp;
extern operation *pp, *pe;

#endif //SIMPSCRIPT_DEFS_H
