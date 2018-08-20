#include <stdio.h>
#include <string.h>
#include "error.h"
#include "nexpr_types.h"
#include "y.tab.h"
#include "basics.h"

struct keyword {
		char   *name;
		int    type;
		int    ivalue;
		double dvalue;
	      };

struct keyword keyword_list[] = {
		{"E",		DOUBLE,	    0,	2.718282},
		{"e",		DOUBLE,	    0,	2.718282},
		{"PI",		DOUBLE,	    0,	3.141593},
		{"pi",		DOUBLE,	    0,	3.141593},
		{"TRUE",	   INT,	 TRUE,	0.0},
		{"true",	   INT,	 TRUE,	0.0},
		{"FALSE",	   INT,	FALSE,	0.0},
		{"false",	   INT, FALSE,  0.0},
		{"BEGIN",	   INT,	    0,  0.0},
		{"begin",	   INT,	    0,  0.0},
		{"END",		   INT,	    0,  0.0},
		{"end",		   INT,	    0,  0.0},
		{"if",		   INT,	    0,  0.0},
		{"else",	   INT,	    0,  0.0},
		};

int num_keywords = sizeof(keyword_list)/sizeof(struct keyword);


int key_exists(name)
char *name;
{
	int i;

	for (i=0; i<num_keywords; i++){
		if (!strcmp(keyword_list[i].name,name)){
			return TRUE;
		}
	}
	return FALSE;
}



int get_keyval(name,token)
char *name;
yytok_type *token;
{
	int i;

	for (i=0; i<num_keywords; i++){
		if (!strcmp(keyword_list[i].name,name)){
			token->type = keyword_list[i].type;
			if (token->type == INT){
				token->val.ivalue = keyword_list[i].ivalue;
			}
			else {
				token->val.dvalue = keyword_list[i].dvalue;
			}
			strcpy(token->string,name);
			return NO_ERROR;
		}
	}
	error(NO_KEY,name);
	return NO_KEY;
}


