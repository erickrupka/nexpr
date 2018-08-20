/* commands */
#define CMD_DEFINE	1
#define CMD_SHOW	2
#define CMD_CLEAR	3
#define CMD_RUN		4
#define CMD_QUIT	5
#define CMD_HELP	6
#define CMD_SHELL	7
#define CMD_SAVE	8
#define CMD_LOG		9
#define CMD_DEBUG	10
#define CMD_VERBOSE	11
#define CMD_READ	12

/* command objects */
#define CMD_VAR		101
#define CMD_FUNCTION	102
#define CMD_FORMAT	103
#define CMD_MACRO	104
#define CMD_VARIABLE	105
#define CMD_VERSION	106

extern void process_cmd();
