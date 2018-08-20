#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "basics.h"
#include "error.h"
#include "main.h"
#include "func.h"
#include "io.h"
#include "control.h"

char *explain[] = {
	"no error",
	"division by zero",
	"type mismatch - cannot convert",
	"attempt to take the logarithm of zero",
	"attempt to take the root of a negative number",
	"attempt to take the arcsine or arccosine of a number greater than 1.0",
	"attempt to raise a negative number to a fractional power",
	"attempt to take factorial of a negative number",
	"attempt to take factorial of a fractional number",
	"cannot use a keyword as a variable or macro name",
	"undeclared variable",
	"no such keyword",
	"no memory",
	"integer overflow",
	"floating overflow",
	"cannot assign a value to anything other than a variable",
	"define of a macro has syntax error",
	"variable already exists",
	"no such macro",
	"unknown command",
	"command does not do this",
	"command not yet implemented",
	"cannot clear all with one command",
	"macros may not call themselves",
	"macro already defined",
	"internal call stack failure",
	"cannot read initialization file",
	"ambigous command substring",
	"ambigous object of a command",
	"cannot open file",
	"file may not read itself",
	"no file name specified",
	"specified thing does not exist",
	"cannot save a keyword",
	"not enough arguments to macro or function",
	"session logging already on",
	"session logging already off",
	"no file specified",
	"bad if conditional expression - wrong type",
	"input abandoned",
	"else found without an if",
	"unknown option",
};

int max_error = sizeof(explain)/sizeof(char *);


void error(int code, char *data)
{
	if (error_result == NO_ERROR){
		error_result = code;
		FPRINTF(stderr,"%s: error: ",prog_name);
	}
	else {
		FPRINTF(stderr,"%s: warning: ",prog_name);
	}
	if ((code >= 0) && (code <= max_error)){
		FPRINTF(stderr,"%s",explain[code]);
		if (data[0] != '\0'){
			FPRINTF(stderr,": %s\n",data);
		}
		else {
			FPRINTF(stderr,"\n");
		}
	}
	else {
		FPRINTF(stderr,"unknown error code: %d\n",code);
	}
	free_stack();
	clear_file_stack();
	/* call stack failure is FATAL!!! */
	if (code == CALL_STACK){
		exit(code);
	}
	strcpy(prompt_string,prog_name);
	inside_block = FALSE;
	last_was_if = FALSE;
	last_if_taken = FALSE;
	eval_on = TRUE;
	longjmp(env,1);
}
