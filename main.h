#include <setjmp.h>

extern char prog_name[];
extern char prompt_string[];
extern char continue_prompt[];
extern char cmd_line[];

extern long max_int, max_fact;
extern double max_double;

extern int error_result;

extern jmp_buf env;

extern char *MALLOC();
extern void bprintf();
extern long dtoi();
