/*****************************************/
/* 		utils.h			 */
/* declarations for functions in utils.c */
/*					 */
/*****************************************/
extern int print();
extern int copy();
extern int add();
extern int subtract();
extern int power();
extern int multiply();
extern int divide();
extern int mod();
extern int uminus();
extern int ones_comp();
extern int not();
extern int conditional();
extern int lor();
extern int land();
extern int bor();
extern int bxor();
extern int band();
extern int test_eq();
extern int test_neq();
extern int rel_gt();
extern int rel_gte();
extern int shiftleft();
extern int shiftright();
extern int op();
extern void set_eval_cond_expr();
extern void clear_eval_cond_expr();
extern void toggle_eval_cond_expr();
extern int eval_cond_expr();
extern int eval_conditional();
