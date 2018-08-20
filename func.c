#include <math.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include "nexpr_types.h"
#include "y.tab.h"
#include "error.h"
#include "utils.h"
#include "keywords.h"
#include "strings.h"
#include "basics.h"
#include "io.h"
#include "main.h"


#define MAX_NAME 80	/* max length of macro name */
#define MAX_CODE 200	/* max number of char in one line of macro code */
#define MAX_ARGS 10	/* $0 - $9 */


typedef struct CODE_TYPE {
	char 			code[MAX_CODE];
	struct CODE_TYPE	*next_code;
} code_type, *code_ptr_type;


typedef struct MACRO_TYPE {
	char 			name[MAX_NAME];
	struct MACRO_TYPE	*next_macro;
	struct CODE_TYPE	*first_code;
	char			args[MAX_ARGS][MAX_NAME];
} macro_type, *macro_ptr_type;


typedef struct STACK_EL_TYPE {
	char			name[MAX_NAME];
	macro_ptr_type		macro_ptr;
	code_ptr_type		code_ptr;
	FILE			*save_file;
	struct STACK_EL_TYPE	*next_el;
} stack_el_type, *stack_el_ptr_type;


macro_ptr_type		first_macro = NULL;
macro_ptr_type		macro_to_run = NULL;
code_ptr_type		code_to_run = NULL;
stack_el_ptr_type	stack_top_ptr = NULL;



char *name_macro_running(void)
{
	if (macro_to_run){
		return macro_to_run->name;
	}
	else {
		return NULL;
	}
}


int macro_running(void)
{
	if (macro_to_run){
		return TRUE;
	}
	else {
		return FALSE;
	}
}


char *replace_args(void)
{
	char 		*next_char = code_to_run->code;
	int  		index = 0;
	static char	line_of_macro[MAX_CODE];

	line_of_macro[0] = '\0';
	while (*next_char){
		if ((*next_char == '$') && (*(next_char+1) >= '0')
			&& (*(next_char+1) <= '9')){
			next_char++;
			if (strlen(macro_to_run->args[*next_char - '0']) == 0){
				error(NO_ARGS,macro_to_run->name);
			}
			strcat(line_of_macro,
				macro_to_run->args[*next_char - '0']);
			index += strlen(macro_to_run->args[*next_char - '0']);
			next_char++;
		}
		else {
			line_of_macro[index] = *next_char;
			index++;
			next_char++;
		}
	}
	line_of_macro[index] = '\0';
	return line_of_macro;
}


int code_is_running(void)
{
	if (code_to_run){
		return TRUE;
	}
	else {
		return FALSE;
	}
}


char *code_running(void)
{
	if (code_to_run){
		return replace_args();
	}
	else {
		return NULL;
	}
}


int more_stack(void)
{
	if (stack_top_ptr != NULL){
		return TRUE;
	}
	else {
		return FALSE;
	}
}



void stack_dump(void)
{
	stack_el_ptr_type	el_ptr;

	el_ptr = stack_top_ptr;
	printf("stack dump:\n");
	while (el_ptr != NULL){
		printf("%s\n",el_ptr->name);
		el_ptr = el_ptr->next_el;
	}
}

void debug(void)
{
	printf("macro running = %d\n",macro_running());
	if (macro_running()){
		printf("name of macro = %s\n",name_macro_running());
	}
	printf("code running = %s\n",code_running());
	if (code_running()){
		printf("code to run = %s\n",code_to_run->code);
	}
	stack_dump();
}

void free_stack(void)
{
	stack_el_ptr_type	el_ptr;

	while (stack_top_ptr != NULL){
		el_ptr = stack_top_ptr;
		free(el_ptr);
		stack_top_ptr = stack_top_ptr->next_el;
	}
	macro_to_run = NULL;
	code_to_run = NULL;
}


void clear_macros(void)
{
	free_stack();
	macro_to_run = NULL;
	code_to_run = NULL;
}



void push_stack(void)
{
	stack_el_ptr_type	new_el;	

	new_el = (stack_el_ptr_type)MALLOC(sizeof(stack_el_type));
	if (new_el == NULL){
		error(NO_MEMORY, "Out of memory");
	}
	strncpy(new_el->name,name_macro_running(),MAX_NAME);
	new_el->macro_ptr = macro_to_run;
	new_el->code_ptr = code_to_run;
	new_el->next_el = stack_top_ptr;
	new_el->save_file = read_this;
	stack_top_ptr = new_el;
}


void pop_stack(void)
{
	stack_el_ptr_type	el_ptr;

	el_ptr = stack_top_ptr;
	if (el_ptr != NULL){
		stack_top_ptr = el_ptr->next_el;
		macro_to_run = el_ptr->macro_ptr;
		code_to_run = el_ptr->code_ptr;
		read_this = el_ptr->save_file;
		if (read_this != NULL){
			reading_file = TRUE;	
		}
		free(el_ptr);
	}
	else {
		error(CALL_STACK,"");
	}
}


int name_in_stack(char *name)
{
	stack_el_ptr_type	el_ptr;
	int			found = FALSE;

	el_ptr = stack_top_ptr;
	while (el_ptr != NULL){
		if (streq(el_ptr->name,name)){
			found = TRUE;
			break;
		}
		el_ptr = el_ptr->next_el;
	}
	return found;
}


void next_code(void)
{
	if (macro_to_run == NULL){
		return;
	}
	else {
		code_to_run = code_to_run->next_code;
	}
}


int macro_exists(char *name)
{
	macro_ptr_type	macro_ptr = first_macro;
	
	while (macro_ptr != NULL){
		if (streq(macro_ptr->name,name)){
			return TRUE;
		}
		macro_ptr = macro_ptr->next_macro;
	}
	return FALSE;
}

void set_args(char *name, char *string, macro_ptr_type macro_ptr)
{
	int arg_num = 1;
	int char_num = 0;

	strcpy(macro_ptr->args[0],name);
	while (*string){
		while (*string == ' '){
			string++;
		}
		while (*string && (*string != ' ')){
			macro_ptr->args[arg_num][char_num] = *string;
			char_num++;
			string++;
		}
		macro_ptr->args[arg_num][char_num] = '\0';
		arg_num++;
		char_num = 0;
	}
}


void run_macro(int cmd_code, int obj_code, char *string)
{
	char		name[MAX_NAME];
	macro_ptr_type	macro_ptr = first_macro;
	int		found = FALSE;
	int 		index;
	
	/* get macro name */
	while (*string == ' '){
		string++;
	}
	index = 0;
	while ((*string != ' ') && *string){
		name[index] = *string;
		string++;
		index++;
	}
	name[index] = '\0';
	/* check macro name */
	if (macro_running() && (streq(name_macro_running(),name))){
		error(MACRO_RECURSION,name);
	}
	if (macro_running() && name_in_stack(name)){
		error(MACRO_RECURSION,name);
	}

	while (macro_ptr != NULL){
		if (streq(macro_ptr->name,name)){
			if (macro_running()){
				push_stack();
			}
			set_args(name,string,macro_ptr);
			macro_to_run = macro_ptr;
			code_to_run = macro_ptr->first_code;
			found = TRUE;
			break;
		}
		else {
			macro_ptr = macro_ptr->next_macro;
		}
	}
	if (!found){
		error(NO_MACRO,name);
	}
}


void show_macro(int cmd_code, int obj_code, char *name)
{
	macro_ptr_type	macro_ptr = first_macro;
	code_ptr_type	code_ptr;
	
	PRINTF("\n");
	if (name[0] != '\0'){
		/* show one macro */
		while (macro_ptr != NULL){
			if (streq(macro_ptr->name,name)){
				PRINTF("BEGIN\n");
				code_ptr = macro_ptr->first_code;
				while (code_ptr != NULL){
					PRINTF("%s\n",code_ptr->code);
					code_ptr = code_ptr->next_code;
				}
				PRINTF("END\n");
				break;
			}
			else {
				macro_ptr = macro_ptr->next_macro;
			}
		}
	}
	else {
		/* show all macros */
		while (macro_ptr != NULL){
			PRINTF("BEGIN %s\n",macro_ptr->name);
			code_ptr = macro_ptr->first_code;
			while (code_ptr != NULL){
				PRINTF("%s\n",code_ptr->code);
				code_ptr = code_ptr->next_code;
			}
			PRINTF("END\n");
			macro_ptr = macro_ptr->next_macro;
			if (macro_ptr != NULL){
				PRINTF("\n");
			}
		}
	}
}


int read_macro(code_ptr_type *code_ptr)
{
	char 		line[200];
	char		*input_result;
	code_ptr_type	new_code_ptr;
	code_ptr_type	end_code_ptr;

	end_code_ptr = *code_ptr;
	while (TRUE){
		if ((initializing || macro_running() || reading_file) 
			&& !verbose_mode){
			;
		}
		else {
			PRINTF("define > ");
		}
		input_result = GETS(line,NO_PROMPT);	
		if (input_result == NULL){
			return FALSE;
		}
		if (streq(line,"END") || streq(line,"end")){
			code_ptr = NULL;
			return TRUE;
		}
		new_code_ptr = (code_ptr_type)MALLOC(sizeof(code_type));	
		if (new_code_ptr == NULL){
			error(NO_MEMORY,"");
		}
		new_code_ptr->next_code = NULL;
		strcpy(new_code_ptr->code,line);
		if (end_code_ptr != NULL){
			end_code_ptr->next_code = new_code_ptr;
		}
		else {
			*code_ptr = new_code_ptr;
		}
		end_code_ptr = new_code_ptr;		
	}
}


void free_macro(macro_ptr_type macro_ptr)
{
	code_ptr_type	code_ptr;
	code_ptr_type	free_this_code;

	if (macro_ptr == NULL){
		return;
	}
	if (macro_ptr->first_code){
		code_ptr = macro_ptr->first_code;
		while (code_ptr != NULL){
			free_this_code = code_ptr;
			code_ptr = code_ptr->next_code;
			free(free_this_code);
		}
	}	
	free(macro_ptr);
}


void define_macro(int cmd_code, int obj_code, char *string)
{
	macro_ptr_type	macro_ptr;
	int		index;

	if (macro_exists(string)){
		error(MACRO_DEFINED,string);
	}	
	macro_ptr = (macro_ptr_type)MALLOC(sizeof(macro_type));
	if (macro_ptr == NULL){
		error(NO_MEMORY,"");
	}
	strncpy(macro_ptr->name,string,MAX_NAME-1);
	index = 0;
	if (key_exists(macro_ptr->name)){
		free_macro(macro_ptr);
		error(KEYWORD,"");
	}
	if (var_exists(macro_ptr->name)){
		free_macro(macro_ptr);
		error(VAR_EXISTS,macro_ptr->name);
	}
	if (read_macro(&(macro_ptr->first_code))){
		macro_ptr->next_macro = first_macro;
		first_macro = macro_ptr;
	}
	else {
		free_macro(macro_ptr);
	}
}


void clear_macro(int cmd_code, int obj_code, char *name)
{
	macro_ptr_type	macro_ptr = first_macro;
	macro_ptr_type  prev_macro_ptr;
	code_ptr_type	code_ptr;

	/* must delete one at a time */
	if (name[0] == '\0'){
		error(NO_CLR_ALL,"");
		return;
	}
	prev_macro_ptr = NULL;	
	while (macro_ptr != NULL){
		if (streq(macro_ptr->name,name)){
			if (prev_macro_ptr == NULL){
				/* it's the first one */
				first_macro = macro_ptr->next_macro;
			}
			else {
				/* not the first one */
				prev_macro_ptr->next_macro = 
					macro_ptr->next_macro;
			}
			free_macro(macro_ptr);
			return;
		}
		else {
			prev_macro_ptr = macro_ptr;
			macro_ptr = macro_ptr->next_macro;
		}
	}
	error(NO_MACRO,name);
}


void save_macro(char *macro, FILE *file)
{
	macro_ptr_type	macro_ptr = first_macro;
	code_ptr_type	code_ptr;
	
	while (macro_ptr != NULL){
		if ((macro[0] != '\0') && !streq(macro,macro_ptr->name)){
			macro_ptr = macro_ptr->next_macro;
			continue;
		}
		FPRINTF(file,":define macro %s\n",macro_ptr->name);
		code_ptr = macro_ptr->first_code;
		while (code_ptr != NULL){
			FPRINTF(file,"%s\n",code_ptr->code);
			code_ptr = code_ptr->next_code;
		}
		FPRINTF(file,"END\n");
		macro_ptr = macro_ptr->next_macro;
	}
}
