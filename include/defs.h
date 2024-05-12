#ifndef SIMPSCRIPT_DEFS_H
#define SIMPSCRIPT_DEFS_H

#include <stdbool.h>

#define MAX_PROGRAM_SIZE 2048
#define MAX_REGISTERS 24
#define MAX_ARGUMENTS 12
#define GLOBAL_STACK_SIZE 16
#define GLOBAL_HEAP_SIZE 4096
#define GLOBAL_BUFF_SIZE 512

#define OPCODE_TABLE                       \
X(INVLD, "")                               \
X(ASGN, "assign")                          \
X(NEWARR, "new_array")                     \
X(RAND, "random")                          \
X(GET, "get_input")                        \
X(ADD, "add")                              \
X(SUBTR, "subtract")                       \
X(MUL, "multiply")                         \
X(DIV, "divide")                           \
X(MOD, "modulo")                           \
X(INCR, "increment")                       \
X(DECR, "decrement")                       \
X(SWP, "swap")                             \
X(BEGLP, "begin_loop",)                    \
X(FOREACH, "for_each")                     \
X(CONT, "continue")                        \
X(BREAK, "break")                          \
X(ENDLP, "end_loop")                       \
X(ENDLPEQ, "end_loop_if_equal")            \
X(ENDLPNE, "end_loop_if_not_equal")        \
X(ENDLPLT, "end_loop_if_less")             \
X(ENDLPLE, "end_loop_if_less_or_equal")    \
X(ENDLPGT, "end_loop_if_greater")          \
X(ENDLPGE, "end_loop_if_greater_or_equal") \
X(IFEQ, "if_equal")                        \
X(IFNE, "if_not_equal")                    \
X(IFLT, "if_less")                         \
X(IFLE, "if_less_or_equal")                \
X(IFGT, "if_greater")                      \
X(IFGE, "if_greater_or_equal")             \
X(ENDIF, "end_if")                         \
X(PRINT, "print")                          \
X(CMNT, ";")                               \
X(DONE, "done")

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

typedef enum {
    #define X(type, ...) type,
    PTR_TYPE_TABLE
    #undef X
} ptr_type_e;

typedef struct {
    void *ptr;
    ptr_type_e type;
    int arr_size;
    int idx;
    bool arr_item;
    bool transient;
} dyn_ptr_t;


typedef struct {
    instructions_e opcode;
    int target[MAX_ARGUMENTS];
    dyn_ptr_t arg_list[MAX_ARGUMENTS];
    int arg_count;
    bool embedded;
} operation;

extern dyn_ptr_t g_registers[MAX_REGISTERS];
extern dyn_ptr_t loop_next;
extern long i_buff;
extern char s_buff[GLOBAL_BUFF_SIZE];
extern operation **j_sp, **j_bp;
extern operation *pp, *pe;

#endif //SIMPSCRIPT_DEFS_H
