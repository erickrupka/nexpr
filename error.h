/****************************************/
/*		error.h			*/
/*					*/
/* defines for error conditions		*/
/*					*/
/****************************************/

#define ERROR		-1	/* general error - msg has already printed */
#define NO_ERROR	0
#define DIV_BY_0	1
#define WRONG_TYPE	2
#define LOG_ZERO	3
#define NEG_ROOT	4
#define ATRIG_RANGE	5
#define FRACT_POWER	6
#define NEG_FACT	7
#define FRACT_FACT	8
#define KEYWORD		9
#define	NO_VAR		10
#define NO_KEY		11
#define NO_MEMORY	12
#define INT_OVERFLOW	13
#define DOUBLE_OVERFLOW	14
#define BAD_ASSIGN	15
#define BAD_DEFINE	16
#define VAR_EXISTS	17
#define NO_MACRO	18
#define UNKNOWN_CMD	19
#define NO_OBJECT	20
#define NOT_IMPLEMENT	21
#define NO_CLR_ALL	22
#define MACRO_RECURSION 23
#define MACRO_DEFINED	24
#define CALL_STACK	25
#define INIT_FILE	26
#define AMBIG_CMD	27
#define AMBIG_OBJ	28
#define FILE_OPEN	29
#define FILE_RECURSION	30
#define NO_FILENAME	31
#define NO_EXIST	32
#define KEYWORD_SAVE	33
#define NO_ARGS		34
#define LOGGING_ON	35
#define LOGGING_OFF	36
#define NO_FILE		37
#define BAD_IF_EXP	38
#define ABANDON		39
#define ELSE_NO_IF	40
#define UNKNOWN_OPTION	41

extern void error(int code, char *s);
