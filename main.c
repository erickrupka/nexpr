#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <setjmp.h>
#include <limits.h>
#include <values.h>
#include <string.h>
#include <getopt.h>
#include "nexpr_types.h"
#include "y.tab.h"
#include "error.h"
#include "utils.h"
#include "func.h"
#include "strings.h"
#include "cmd_defs.h"
#include "basics.h"
#include "format.h"
#include "io.h"
#include "control.h"

extern char *getenv();


char 	cmd_line[256];
char 	prog_name[256];
char 	prompt_string[256];
char	continue_prompt[256];
long	max_int, max_fact;
double	max_double;
double  log_max_int, log_max_double;
int	used_argv;
int	error_result;
jmp_buf	env;


/* MALLOC does malloc, checks for success, and zeros out malloc'ed space */
char *MALLOC(int bytes)
{
	char *space;
	int  byte_num;

	space = (char *)malloc(bytes);
	if (space == NULL){
		return NULL;
	}
	for (byte_num=0; byte_num<bytes; byte_num++){
		*(space + byte_num) = '\0';
	}
	return space;
}


long dtoi(double num)
{
	if (ABS(num) > max_int){
		error(INT_OVERFLOW,"");
		return 0;
	}
	if (num >= 0.0){
		return (long)(num + 0.5);
	}
	else {
		return (long)(num - 0.5);
	}
}



int simple_overflow(int op, yytok_type *x, yytok_type *y)
{
	long	xint, yint;
	double	xdouble, ydouble;
	int 	type;

	if (x->type == DOUBLE || (y->type == DOUBLE)){
		type = DOUBLE;
	}
	else {
		type = INT;
	}
	if (x->type == DOUBLE){
		xdouble = x->val.dvalue;
	}
	else if (type == DOUBLE){
		xdouble = (double)x->val.ivalue;
	}
	else {
		xint = x->val.ivalue;
	}
	if (y->type == DOUBLE){
		ydouble = y->val.dvalue;
	}
	else if (type == DOUBLE){
		ydouble = (double)y->val.ivalue;
	}
	else {
		yint = y->val.ivalue;
	}

	switch (op){
		case (int)'+':
			if (type == DOUBLE){
				if ((xdouble > 0.0) && (ydouble > 0.0) &&
					((max_double - xdouble) < ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
				else if ((xdouble < 0.0) && (ydouble < 0.0) &&
					((max_double + xdouble) < -ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if ((xint > 0) && (yint > 0) &&
					((max_int - xint) < yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
				else if ((xint < 0) && (yint < 0) &&
					((max_int + xint) < -yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			break;

		case (int)'-':
			if (type == DOUBLE){
				if ((xdouble < 0.0) && (ydouble > 0.0) &&
					((max_double + xdouble) < ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
				if ((xdouble > 0.0) && (ydouble < 0.0) &&
					((max_double - xdouble) < -ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if ((xint < 0) && (yint > 0) &&
					((max_int + xint) < yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
				if ((xint > 0) && (yint < 0) &&
					((max_int - xint) < -yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			break;

		case (int)'*': 
			if (type == DOUBLE){
				if ((log(ABS(xdouble)) + log(ABS(ydouble))) > 
					log_max_double){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if ((log((double)ABS(xint)) 
					+ log((double)ABS(yint))) > 
					log_max_int){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}	
			break;

		case OP_EXP:
			if (type == DOUBLE){
				if (xdouble > log_max_double){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if (xint > log_max_int){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			break;

		case OP_SQR:
			if (type == DOUBLE){
				if ((log(ABS(xdouble)) * 2) > log_max_double){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if ((log((double)ABS(xint)) * 2) > log_max_int){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}	
			break;

		case OP_FACT:
			if (type == DOUBLE){
				if (dtoi(xdouble) > max_fact){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if (xint > max_fact){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			break;

		case POWER:
			if (type == INT){
				xdouble = (double)xint;
				ydouble = (double)yint;
			}
			/* check if next check will cause overflow */
			/* but first check if we can take log(log(xdouble)) */
			if (log(ABS(xdouble)) != 0.0){
				/* NOW check that check */
				if ((log(ABS(log(ABS(xdouble)))) + 
					log(ABS(ydouble))) > 
					log_max_double){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			/* check for overflow */
			if ((log(ABS(xdouble)) * ydouble) > log_max_double){
				error(DOUBLE_OVERFLOW,"");
				return TRUE;
			}
			break;

		case AS_ADD:
			if (type == DOUBLE){
				if ((xdouble > 0.0) && (ydouble > 0.0) &&
					((max_double - xdouble) < ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
				else if ((xdouble < 0.0) && (ydouble < 0.0) &&
					((max_double + xdouble) < -ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if ((xint > 0) && (yint > 0) &&
					((max_int - xint) < yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
				else if ((xint < 0) && (yint < 0) &&
					((max_int + xint) < -yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			break;

		case AS_SUB:
			if (type == DOUBLE){
				if ((xdouble < 0.0) && (ydouble > 0.0) &&
					((max_double + xdouble) < ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
				if ((xdouble > 0.0) && (ydouble < 0.0) &&
					((max_double - xdouble) < -ydouble)){
					error(DOUBLE_OVERFLOW,"");
					return TRUE;
				}
			}
			else {
				if ((xint < 0) && (yint > 0) &&
					((max_int + xint) < yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
				if ((xint > 0) && (yint < 0) &&
					((max_int - xint) < -yint)){
					error(INT_OVERFLOW,"");
					return TRUE;
				}
			}
			break;

	}
	return FALSE;
}

int overflow(int op, yytok_type *x, yytok_type *y)
{
	int result;

	result = simple_overflow(op,x,y);
	if (result == TRUE){
		if ((x->type == INT) && (y->type == INT)){
			x->type = DOUBLE;
			x->val.dvalue = (double)x->val.ivalue;
			y->type = DOUBLE;
			y->val.dvalue = (double)y->val.ivalue;
			return simple_overflow(op,x,y);
		}
	}
	return result;
}



void find_max(void)
{
	long x = 1;

	max_int = 1;
        while (x > 0){
		x <<= 1;
		max_int = (max_int << 1) + 1;
	}
	max_int &= ~x;
	max_double = (double)MAXFLOAT;
	log_max_int = log((double)max_int);
	log_max_double = log(max_double);

	max_fact = 1;
	x = 1;
	while ((max_int/x) > max_fact){
		max_fact++;
		x *= max_fact;
	}
}


void collect_expr(int first_arg, int argc, char *argv[])
{
	int i;
	
	cmd_line[0] = '\0';
	for (i=first_arg; i<argc; i++){
		strcpy(&cmd_line[strlen(cmd_line)],argv[i]);
		cmd_line[strlen(cmd_line)] = ' ';
		cmd_line[strlen(cmd_line)] = '\0';
	}
	if (cmd_line[0] == '\0'){
		used_argv = FALSE;
	}
	else
		used_argv = TRUE;
}


void fix_optind(int *optind, char *argv[])
{
	char *arg;

	if (*optind > 0){
		arg = argv[*optind-1];
		if ((arg[1] >= '0') && (arg[1] <= '9')){
			(*optind)--;
		}
		else {
			error(UNKNOWN_OPTION,arg);
		}
	}
}


void args(int argc, char *argv[])
{
	int c;
	int found_all_opts = FALSE;

	opterr = 0;
	optind = 1;
	while ((c = getopt(argc,argv,"vf:")) != EOF){
		switch (c){
			case 'v' : FPRINTF(stderr,"%s\n",VERSION);
				   break;
			case 'f' : define_format(CMD_DEFINE,CMD_FORMAT,optarg);
				   break;
			case '?' : found_all_opts = TRUE;
				   fix_optind(&optind,argv);
				   break;
			default  : optind++; 
				   found_all_opts = TRUE;
		}
		if (found_all_opts)
			break;
	}
	collect_expr(optind,argc,argv);
}


int main(int argc, char *argv[])
{
	char 	*input_result;
	int 	done = FALSE;

	find_max();
	strcpy(prog_name,argv[0]);
	strcpy(prompt_string,argv[0]);
	continue_prompt[0] = '\0';
	while (strlen(continue_prompt) < strlen(prog_name)){
		strcat(continue_prompt,"-");
	}
	init();
	set_eval_flag();
	error_result = setjmp(env);
	if (error_result == 0){
		args(argc,argv);
	}
	else {
		exit(error_result);
	}
	while (!done){
		clear_eval_cond_expr();
		toggle_eval_cond_expr();
		error_result = NO_ERROR;
		if (!used_argv){
			input_result = GETS(cmd_line,PROMPT);
			if (input_result == NULL){ /* ^D */
				PRINTF("\n");
				exit(0);
			}
		}
		/* setjmp returns 0 when it is called.	*/
		/* It returns something other than 0  	*/
		/* when longjmp returns to it		*/
		error_result = setjmp(env);
		if (error_result == 0){
			if (cmd_line[0] == ':'){
				process_cmd(cmd_line+1);
			}
			else if (cmd_line[0] == '?'){
				process_cmd(cmd_line);
			}
			else {
				//cmd_line[strlen(cmd_line)] = '\n';
				while (cmd_line[strlen(cmd_line)] == '\n'){
					cmd_line[strlen(cmd_line)] = '\0';
				}
				yyparse();
			}
		}
		else {
			/* There was an error - reset variables to */
			/* their values before this computation.   */
			PRINTF("0\n");
			reset_vars();
		}
		if (used_argv)
			done = TRUE;
		else {
			clear_used_vars();
			reset_input();
			if (!((initializing || macro_running() || reading_file) 
				&& !verbose_mode)){
				PRINTF("\n");
			}
		}
	}
	exit(error_result);
}

void yyerror(char *s)
{
	FPRINTF(stderr,"%s\n",s);
	longjmp(env,1);
}
