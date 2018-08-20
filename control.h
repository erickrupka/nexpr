extern int eval_on;
extern int inside_block;
extern int last_was_if;
extern int last_if_taken;

extern void set_eval_flag();
extern void clear_eval_flag();
extern void test_eval_flag();

extern void start_if();
extern void end_if();
extern void start_else();
extern void end_else();
extern void start_block();
extern void end_block();
