#ifndef SIMPSCRIPT_GLOBALS_H
#define SIMPSCRIPT_GLOBALS_H

extern long g_registers[MAX_REGISTERS];
extern char *s_registers[MAX_REGISTERS];
extern char s_buff[GLOBAL_BUFF_SIZE];
extern long *e_sp, *e_bp;
extern char **j_sp, **j_bp;

#endif //SIMPSCRIPT_GLOBALS_H
