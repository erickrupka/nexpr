#include <stdio.h>
#include "basics.h"
#include "error.h"
#include "nexpr_types.h"
#include "utils.h"
#include "strings.h"
#include "main.h"
#include "format.h"
#include "io.h"
#include "func.h"
#include "keywords.h"

int show_var(int cmd_code, int obj_code, char *string)
{
	yytok_type token_in, token_out;

	if (*string){
		/* show just one */
		if (var_exists(string)){
			strcpy(token_in.string,string);
			get_val(&token_out,token_in);
			print(token_out,FORCED);
		}
		else {
			error(NO_VAR,string);
		}
	}
	else {
		/* show all */
		show_vars(format);
	}
}


int clear_var(int cmd_code, int obj_code, char *string)
{
	if (*string){
		if (var_exists(string)){
			delete_var(string);
		}
		else {
			error(NO_VAR,string);
		}
	}
	else {
		clear_vars();
	}
}


void save(int cmd_code, int obj_code, char *string)
{
	FILE		*file;
	char		file_name[100];
	char		obj_name[100];
	int		index;
	
	while (*string == ' '){
		string++;
	}
	index = 0;
	while (*string && (*string != ' ')){
		file_name[index] = *string;
		string++;
		index++;
	}
	file_name[index] = '\0';
	while (*string == ' '){
		string++;
	}
	index = 0;
	while (*string && (*string != ' ')){
		obj_name[index] = *string;
		string++;
		index++;
	}
	obj_name[index] = '\0';

	if (file_name[0] == '\0'){
		error(NO_FILENAME,"");
	}
	file = fopen(file_name,"a");
	if (file == NULL){
		error(FILE_OPEN,file_name);
	}

	if (obj_name[0] != '\0'){
		if (macro_exists(obj_name)){
			save_macro(obj_name,file);
		}
		else if (var_exists(obj_name)){
			save_var(obj_name,file);
		}
		else if (key_exists(obj_name)){
			error(KEYWORD_SAVE,obj_name);
		}
		else {
			error(NO_EXIST,obj_name);
		}
	}
	else {
		save_var(obj_name,file);
		save_macro(obj_name,file);
	}

	fclose(file);
}
