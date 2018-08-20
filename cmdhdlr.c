#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdhdlr.h"
#include "cmd_defs.h"
#include "error.h"
#include "basics.h"
#include "cmds.h"
#include "format.h"
#include "func.h"
#include "help.h"
#include "io.h"
#include "control.h"

int quit(int cmd_code, int obj_code, char *string)
{
	exit(0);
}


int shell(int cmd_code, int obj_code, char *string)
{
	system(string);
}


int show_version(int cmd_code, int obj_code, char *string)
{
	PRINTF("%s",VERSION);
}


/* object */

cmd_obj_type obj_define[] = {
	{"function",	CMD_FUNCTION,	NULL},
	{"macro",	CMD_MACRO,	(char (*)())define_macro},
};
int num_obj_define = sizeof(obj_define)/sizeof(cmd_obj_type);

cmd_obj_type obj_show[] = {
	{"function",	CMD_FUNCTION,	NULL},
	{"macro",	CMD_MACRO,	(char (*)())show_macro},
	{"variable",	CMD_VARIABLE,	(char (*)())show_var},
	{"format",	CMD_FORMAT,	(char (*)())show_format},
	{"version",	CMD_VERSION,	(char (*)())show_version},
};
int num_obj_show = sizeof(obj_show)/sizeof(cmd_obj_type);

cmd_obj_type obj_clear[] = {
	{"variable",	CMD_VARIABLE,	(char (*)())clear_var},
	{"function",	CMD_FUNCTION,	NULL},
	{"macro",	CMD_MACRO,	(char (*)())clear_macro},
};
int num_obj_clear = sizeof(obj_clear)/sizeof(cmd_obj_type);

cmd_obj_type obj_run[] = {
	{"",	CMD_MACRO,	(char (*)())run_macro},
};
int num_obj_run = sizeof(obj_run)/sizeof(cmd_obj_type);

cmd_obj_type obj_save[] = {
	{"",	CMD_SAVE,	(char (*)())save},
};
int num_obj_save = sizeof(obj_save)/sizeof(cmd_obj_type);

cmd_obj_type obj_shell[] = {
	{"",		CMD_SHELL,	(char (*)())shell},
};
int num_obj_shell = sizeof(obj_shell)/sizeof(cmd_obj_type);

cmd_obj_type obj_log[] = {
	{"on",		CMD_LOG,	(char (*)())log_on},
	{"off",		CMD_LOG,	(char (*)())log_off},
	{"ON",		CMD_LOG,	(char (*)())log_on},
	{"OFF",		CMD_LOG,	(char (*)())log_off},
};
int num_obj_log = sizeof(obj_log)/sizeof(cmd_obj_type);


cmd_obj_type obj_help[] = {
	{"",		CMD_HELP,	(char (*)())help},
};
int num_obj_help = sizeof(obj_help)/sizeof(cmd_obj_type);


cmd_obj_type obj_quit[] = {
	{"",		CMD_QUIT,	(char (*)())quit},
};
int num_obj_quit = sizeof(obj_quit)/sizeof(cmd_obj_type);

cmd_obj_type obj_debug[] = {
	{"",		CMD_DEBUG,	(char (*)())debug},
};
int num_obj_debug = sizeof(obj_debug)/sizeof(cmd_obj_type);


cmd_obj_type obj_verbose[] = {
	{"on",		CMD_VERBOSE,	(char (*)())verbose_on},
	{"ON",		CMD_VERBOSE,	(char (*)())verbose_on},
	{"off",		CMD_VERBOSE,	(char (*)())verbose_off},
	{"OFF",		CMD_VERBOSE,	(char (*)())verbose_off},
};
int num_obj_verbose = sizeof(obj_verbose)/sizeof(cmd_obj_type);


cmd_obj_type obj_read[] = {
	{"",		CMD_READ,	(char (*)())read_file},
};
int num_obj_read = sizeof(obj_read)/sizeof(cmd_obj_type);

cmd_obj_type obj_format[] = {
	{"",		CMD_FORMAT,	(char (*)())define_format},
};
int num_obj_format = sizeof(obj_format)/sizeof(cmd_obj_type);

/* commands */
cmd_tbl_type cmd_tbl[] = {
	{"define",	CMD_DEFINE,	obj_define,	&num_obj_define	},
	{"show",	CMD_SHOW,	obj_show,	&num_obj_show	},
	{"clear",	CMD_CLEAR,	obj_clear,	&num_obj_clear	},
	{"run",		CMD_RUN,	obj_run,	&num_obj_run	},
	{"quit",	CMD_QUIT,	obj_quit,	&num_obj_quit	},
	{"help",	CMD_HELP,	obj_help,	&num_obj_help	},
	{"exit",	CMD_QUIT,	obj_quit,	&num_obj_quit	},
	{"!",		CMD_SHELL,	obj_shell,	&num_obj_shell	},
	{"save",	CMD_SAVE,	obj_save,	&num_obj_save	},
	{"log",		CMD_LOG,	obj_log,	&num_obj_log	},
	{"debug",	CMD_DEBUG,	obj_debug,	&num_obj_debug  },
	{"verbose",	CMD_VERBOSE,	obj_verbose,	&num_obj_verbose},
	{"read",	CMD_READ,	obj_read,	&num_obj_read	},
	{"format",	CMD_FORMAT,	obj_format,	&num_obj_format },
};
int num_cmds = sizeof(cmd_tbl)/sizeof(cmd_tbl_type);



void process_cmd(char *string)
{
	char 			word[100];
	char 			*word_ptr = word;
	int  			cmd_num;
	int  			obj_num;
	cmd_tbl_ptr_type	cmd_ptr = cmd_tbl;
	cmd_obj_ptr_type	obj_ptr;
	int			found = FALSE;
	int 			index;
	
	if (! *string){
		return;
	}

	/* get rid of leading spaces */
	while (*string && (*string == ' ')){
		string++;
	}

	/* get the command from the string */
	while (*string && (*string != ' ')){
		*word_ptr = *string;
		word_ptr++;
		string++;
		if (*(string-1) == '!'){
			break;
		}
	}
	*word_ptr = '\0';

	cmd_num = num_cmds;
	/* check if it is a valid command */
	for (index=0; index<num_cmds; index++){
		if (strneq(cmd_ptr->name,word,strlen(word))){
			if (found == TRUE){
				error(AMBIG_CMD,word);
			}
			else {
				found = TRUE;
				cmd_num = index;
			}
		}
		cmd_ptr++;
	}
	if (cmd_num >= num_cmds){
		error(UNKNOWN_CMD,word);
	}
	cmd_ptr = cmd_tbl + cmd_num;

	obj_ptr = cmd_ptr->obj_ptr;
	word[0] = '\0';
	/* if object is needed, get the object from the string */
	if (obj_ptr->name[0] != '\0'){
		word_ptr = word;
		if (*string){
			string++;
		}
		/* get rid of leading spaces */
		while (*string && (*string == ' ')){
			string++;
		}
		while (*string && (*string != ' ')){
			*word_ptr = *string;
			word_ptr++;
			string++;
		}
		*word_ptr = '\0';
	}

	/* check if it is a valid object */
	found = FALSE;
	obj_num = *(cmd_ptr->num_objs);
	for (index=0; index<*(cmd_ptr->num_objs); index++){
		if (strneq(obj_ptr->name,word,strlen(word))){
			if (found == TRUE){
				error(AMBIG_OBJ,word);
			}
			else {
				found = TRUE;
				obj_num = index;
			}
		}
		obj_ptr++;
	}
	if (obj_num >= *(cmd_ptr->num_objs)){
		error(NO_OBJECT,word);
	}
	obj_ptr = cmd_ptr->obj_ptr + obj_num;

	if (*string && (*(string-1) != '!')){
		string++;
	}
	/* get rid of leading spaces */
	while (*string && (*string == ' ')){
		string++;
	}
	/* remove trailing spaces */
	word_ptr = string + strlen(string) - 1;
	while ((word_ptr > string) && (*word_ptr == ' ')){
		*word_ptr = '\0';
		word_ptr--;
	}

	/* if in part of "if" that shouldn't be executed, return */
	/* without doing anything.				 */
	if (!eval_on){
		return;
	}
	/* call the function */
	if (obj_ptr->func){
		obj_ptr->func(cmd_ptr->code,obj_ptr->code,string);
	}
	else {
		error(NOT_IMPLEMENT,"");
	}
}
