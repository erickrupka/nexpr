#include <stdio.h>
#include <string.h>
#include "nexpr_types.h"
#include "y.tab.h"
#include "error.h"
#include "utils.h"
#include "keywords.h"
#include "basics.h"
#include "main.h"
#include "format.h"
#include "io.h"
#include "func.h"
#include "control.h"

struct string {
		char 		name[80];
		int		type;
		int		oldtype;
		union {
			double	dvalue;
			long	ivalue;
		} val;
		union {
			double	dvalue;
			long	ivalue;
		} oldval;
		int 		used;
		int 		assigned;
		int		incs;
		struct string 	*next_var;
	      };

struct string *first = NULL;

int var_exists(name)
char *name;
{
    struct string *tmp;

    if (key_exists(name)){
	return TRUE;
    }
    tmp = first;
    while ((tmp != NULL) && strcmp(tmp->name,name)){
	tmp = tmp->next_var;
    }
    if (tmp != NULL){
	return TRUE;
    }
    else {
	return FALSE;
    }
}


void declare(name)
char *name;
{
    struct string *tmp;

    tmp = (struct string *)MALLOC(sizeof(struct string));
    if (tmp == NULL){
	error(NO_MEMORY,"");
    }
    else {
	strcpy(tmp->name,name);
	tmp->assigned = FALSE;
	tmp->used = FALSE;
	tmp->type = INT;
	tmp->val.ivalue = 0;
	tmp->next_var = first;
	first = tmp;
    } 
}


int assign(result,var,exp,code)
yytok_type	*result,var,exp;
int		code;
{
    struct string *tmp;

    if (var.type != VAR){
	error(BAD_ASSIGN,"");
	return BAD_ASSIGN;
    }
    /* if in part of "if" that shouldn't be executed */
    if (!eval_on){
	return copy(result,exp);
    }
    /* if in part of ?: that shouldn't be executed */
    if (!eval_conditional()){
	return copy(result,exp);
    }
    if (key_exists(var.string)){
	error(KEYWORD,"");
    }
    if (macro_exists(var.string)){
	error(MACRO_DEFINED,var.string);
    }
    if (!var_exists(var.string)){
	if (code != AS_NORM){
	    error(NO_VAR,"");
	}
	else {
	    declare(var.string);
	}
    }
    tmp = first;
    while ((tmp != NULL) && strcmp(tmp->name,var.string)){
	tmp = tmp->next_var;
    }
    if (tmp != NULL){
	switch(code){

	    case AS_NORM:
		tmp->type = exp.type;
		if (tmp->type == INT){
		    tmp->val.ivalue = exp.val.ivalue;
		}
		else {
		    tmp->val.dvalue = exp.val.dvalue;
		}
		break;

	    case AS_ADD:
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue += exp.val.ivalue;
		    }
		    else {
			tmp->type = DOUBLE;
			tmp->val.dvalue = (double)tmp->val.ivalue + 
			    exp.val.dvalue;
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->type = DOUBLE;
			tmp->val.dvalue = tmp->val.dvalue +
			    (double)exp.val.ivalue;
		    }
		    else {
			tmp->val.dvalue += exp.val.dvalue;
		    }
		}
		break;

	    case AS_SUB:
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue -= exp.val.ivalue;
		    }
		    else {
			tmp->type = DOUBLE;
			tmp->val.dvalue = (double)tmp->val.ivalue -
			    exp.val.dvalue;
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->type = DOUBLE;
			tmp->val.dvalue = tmp->val.dvalue -
			    (double)exp.val.ivalue;
		    }
		    else {
			tmp->val.dvalue -= exp.val.dvalue;
		    }
		}
		break;

	    case AS_OR:
		tmp->type = DOUBLE;
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue |= exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue |= dtoi(exp.val.dvalue);
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->val.ivalue = dtoi(tmp->val.dvalue) |
			    exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = dtoi(tmp->val.dvalue) | 
			    dtoi(exp.val.dvalue);
		    }
		}
		break;

	    case AS_XOR:
		tmp->type = DOUBLE;
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue ^= exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue ^= dtoi(exp.val.dvalue);
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->val.ivalue = dtoi(tmp->val.dvalue) ^
			    exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = dtoi(tmp->val.dvalue) ^
			    dtoi(exp.val.dvalue);
		    }
		}
		break;

	    case AS_AND:
		tmp->type = DOUBLE;
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue &= exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue &= dtoi(exp.val.dvalue);
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->val.ivalue = dtoi(tmp->val.dvalue) &
			    exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = dtoi(tmp->val.dvalue) &
			    dtoi(exp.val.dvalue);
		    }
		}
		break;

	    case AS_RIGHT:
		tmp->type = DOUBLE;
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue >>= exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue >>= dtoi(exp.val.dvalue);
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->val.ivalue = dtoi(tmp->val.dvalue) >>
			    exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = dtoi(tmp->val.dvalue) >>
			    dtoi(exp.val.dvalue);
		    }
		}
		break;

	    case AS_LEFT:
		tmp->type = DOUBLE;
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue <<= exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue <<= dtoi(exp.val.dvalue);
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->val.ivalue = dtoi(tmp->val.dvalue) <<
			    exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = dtoi(tmp->val.dvalue) <<
			    dtoi(exp.val.dvalue);
		    }
		}
		break;

	    case AS_MUL:
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue *= exp.val.ivalue;
		    }
		    else {
			tmp->type = DOUBLE;
			tmp->val.dvalue = (double)tmp->val.ivalue * 
			    exp.val.dvalue;
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->type = DOUBLE;
			tmp->val.dvalue = tmp->val.dvalue *
			    (double)exp.val.ivalue;
		    }
		    else {
			tmp->val.dvalue *= exp.val.dvalue;
		    }
		}
		break;

	    case AS_DIV:
		if (exp.type == INT){
		    if (exp.val.ivalue == 0){
			error(DIV_BY_0,"");
			return DIV_BY_0;
		    }
		}
		else {
		    if (exp.val.dvalue == 0.0){
			error(DIV_BY_0,"");
			return DIV_BY_0;
		    }
		}
		if (tmp->type == INT){
		    if (exp.type == INT){
			tmp->val.ivalue /= exp.val.ivalue;
		    }
		    else {
			tmp->type = DOUBLE;
			tmp->val.dvalue = (double)tmp->val.ivalue /
			    exp.val.dvalue;
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->type = DOUBLE;
			tmp->val.dvalue = tmp->val.dvalue /
			    (double)exp.val.ivalue;
		    }
		    else {
			tmp->val.dvalue /= exp.val.dvalue;
		    }
		}
		break;

	    case AS_MOD:
		if (exp.type == INT){
		    if (exp.val.ivalue == 0){
			error(DIV_BY_0,"");
			return DIV_BY_0;
		    }
		}
		else {
		    if (exp.val.dvalue == 0.0){
			error(DIV_BY_0,"");
			return DIV_BY_0;
		    }
		}
		if (tmp->type == INT){
		    tmp->type = DOUBLE;
		    if (exp.type == INT){
			tmp->val.ivalue %= exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = tmp->val.ivalue %
			    dtoi(exp.val.dvalue);
		    }
		}
		else {
		    if (exp.type == INT){
			tmp->val.ivalue = dtoi(tmp->val.dvalue) %
			    exp.val.ivalue;
		    }
		    else {
			tmp->val.ivalue = dtoi(tmp->val.dvalue) %
			    dtoi(exp.val.dvalue);
		    }
		}
		break;

	    case AS_INC:
		tmp->incs++;
		break;	

	    case AS_DEC:
		tmp->incs--;
		break;	
	}
	tmp->assigned = TRUE;
	tmp->used = TRUE;
	result->type = tmp->type;
	if (tmp->type == INT){
	    result->val.ivalue = tmp->val.ivalue;
	}
	else {
	    result->val.dvalue = tmp->val.dvalue;
	}
	strcpy(result->string,tmp->name);
	return NO_ERROR;
    }
    error(NO_VAR,tmp->name); 
    return NO_VAR;
}


int get_val(result,from)
yytok_type *result,from;
{
	struct string *tmp;

	if (key_exists(from.string)){
		return get_keyval(from.string,result);
	}
	tmp = first;
	while ((tmp != NULL) && strcmp(tmp->name,from.string)){
		tmp = tmp->next_var;
	}
	if (tmp != NULL){
		result->type = tmp->type;
		if (result->type == INT){
			result->val.ivalue = tmp->val.ivalue;
		}
		else {
			result->val.dvalue = tmp->val.dvalue;
		}
		return NO_ERROR;
	}
	else {
		error(NO_VAR,from.string);
		return NO_VAR;
	}
}


void show_vars(format)
char *format;
{
    struct string *tmp;

    tmp = first;
    while (tmp != NULL){
	PRINTF("%s = ",tmp->name);
	if (tmp->assigned){
	    switch (formatchar){

		case 'f': 
		    if (tmp->type == INT){
			PRINTF(format,(double)tmp->val.ivalue);
		    }
		    else {
			PRINTF(format,tmp->val.dvalue);
		    }
		    break;

		case 'i': 
		    if (tmp->type == INT){
			PRINTF(format,tmp->val.ivalue);
		    }
		    else {
			PRINTF(format,dtoi(tmp->val.dvalue));
		    }
		    break;

		case 'b': 
		    if (tmp->type == INT){
			BPRINTF(tmp->val.ivalue);
		    }
		    else {
			BPRINTF(dtoi(tmp->val.dvalue));
		    }
		    break;

		case 'c': 
		    if (tmp->type == INT){
			PRINTF(format,(char)tmp->val.ivalue);
		    }
		    else {
			PRINTF(format,(char)dtoi(tmp->val.dvalue));
		    }
		    break;
	    }
	}
	tmp = tmp->next_var;
	PRINTF("\n");
    }
}


void show_used_vars(format)
char *format;
{
    struct string *tmp;

    if (!eval_on){
	return;
    }
    if ((macro_running() || initializing || reading_file) && !verbose_mode){
	return;
    }
    if (inside_block && !verbose_mode){
	return;
    }
    tmp = first;
    while (tmp != NULL){
	if (tmp->used){
	    PRINTF("%s = ",tmp->name);
	    if (tmp->assigned){
		switch (formatchar){
		    case 'i':
			if (tmp->type == INT){
		    	   PRINTF(format,tmp->val.ivalue);
			}
			else {
		    	    PRINTF(format,dtoi(tmp->val.dvalue));
			}
			break;

		    case 'f':
			if (tmp->type == INT){
		    	   PRINTF(format,(double)tmp->val.ivalue);
			}
			else {
		    	    PRINTF(format,tmp->val.dvalue);
			}
			break;

		    case 'b':
			if (tmp->type == INT){
		    	   BPRINTF(tmp->val.ivalue);
			}
			else {
		    	    BPRINTF(dtoi(tmp->val.dvalue));
			}
			break;

		    case 'c':
			if (tmp->type == INT){
		    	   PRINTF(format,(char)tmp->val.ivalue);
			}
			else {
		    	    PRINTF(format,(char)dtoi(tmp->val.dvalue));
			}
			break;

		    default:
			PRINTF("cannot print value: bad format\n");
		}
	    }
	    else {
		PRINTF("no value\n");
	    }
	    PRINTF("\n");
    	}
    	tmp = tmp->next_var;
    }
}


int var_assigned(name)
char *name;
{
	struct string *tmp;
	
	if (key_exists(name)){
		return TRUE;
	}
	tmp = first;
	while (tmp != NULL){
		if (!strcmp(tmp->name,name))
			return TRUE;
		tmp = tmp->next_var;
	}
	return FALSE;
}


void clear_used_vars()
{
	struct string *tmp;

	tmp = first;
	while (tmp != NULL){
		tmp->used = FALSE;
		tmp = tmp->next_var;
	}
}


void clear_vars()
{
	struct string *tmp, *next;

	tmp = first;
	while (tmp != NULL){
		next = tmp->next_var;
		free((char *)tmp);
		tmp = next;
	}
	first = NULL;
}

int delete_var(name)
char *name;
{
	struct string *prev, *this;

	prev = NULL;
	this = first;
	while (this && !streq(this->name,name)){
		prev = this;
		this = this->next_var;
	}
	if (this){
		/* found it */
		if (prev){
			/* it's not the first var */
			prev->next_var = this->next_var;
			free(this);
		}
		else {
			/* it's the first var */
			first = this->next_var;
			free(this);
		}
		return NO_ERROR;
	}
	else {
		error(NO_VAR,name);
	}
}

int update_vars()
{
	struct string *tmp;

	tmp = first;
	while (tmp != NULL){
		tmp->oldtype = tmp->type;
		if (tmp->type == INT){
			tmp->oldval.ivalue = tmp->val.ivalue += tmp->incs;
		}
		else {
			tmp->oldval.dvalue = tmp->val.dvalue += 
				(double)tmp->incs;
		}
		tmp->incs = 0;
		tmp = tmp->next_var;
	}
}



int reset_vars()
{
	struct string *tmp;

	tmp = first;
	while (tmp != NULL){
		tmp->type = tmp->oldtype;
		if (tmp->type == INT){
			tmp->val.ivalue = tmp->oldval.ivalue;
		}
		else {
			tmp->val.dvalue = tmp->oldval.dvalue;
		}
		tmp->incs = 0;
		tmp = tmp->next_var;
	}
}


void save_var(name,file)
char *name;
FILE *file;
{
    struct string *tmp;

    tmp = first;
    while (tmp != NULL){
	if ((name[0] != '\0') && !streq(name,tmp->name)){
	    tmp = tmp->next_var;
	    continue;
	}
	FPRINTF(file,"%s = ",tmp->name);
	if (tmp->assigned){
	    switch (formatchar){

		case 'f': 
	            if (tmp->type == INT){
		 	FPRINTF(file,format,(double)tmp->val.ivalue);
		    }
		    else {
			FPRINTF(file,format,tmp->val.dvalue);
		    }
		    break;

		case 'i': 
		    if (tmp->type == INT){
			FPRINTF(file,format,tmp->val.ivalue);
		    }
		    else {
			FPRINTF(file,format,dtoi(tmp->val.dvalue));
		    }
		    break;

		case 'b': 
		    if (tmp->type == INT){
			BFPRINTF(file,tmp->val.ivalue);
		    }
		    else {
			BFPRINTF(file,dtoi(tmp->val.dvalue));
		    }
		    break;

		case 'c': 
		    if (tmp->type == INT){
			FPRINTF(file,format,(char)tmp->val.ivalue);
		    }
		    else {
			FPRINTF(file,format,(char)dtoi(tmp->val.dvalue));
		    }
		    break;
	    }
	}
	tmp = tmp->next_var;
	FPRINTF(file,"\n");
    }
}
