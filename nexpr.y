%{

#include <ctype.h>
#include <stdio.h>
#include "nexpr_types.h"
#include "error.h"
#include "utils.h"
#include "strings.h"
#include "func.h"
#include "format.h"
#include "basics.h"
#include "io.h"
#include "control.h"

extern int yylex();

int yydebug=1; 

%}

%start prog

%union {
	yytok_type tok;
       }

%type <tok> prog block statement_list statement exp base if_exp else_exp end

%token <tok> IF
%token <tok> ELSE
%token <tok> STRING
%token <tok> FUNCTION
%token <tok> VAR
%token <tok> QMARK
%token <tok> GARBAGE
%token <tok> WHITESPACE
%token <tok> DOUBLE
%token <tok> INT
%token <tok> END

%token <tok.val.ivalue> OP_LOG	
%token <tok.val.ivalue> OP_LOG10
%token <tok.val.ivalue> OP_LN
%token <tok.val.ivalue> OP_EXP	
%token <tok.val.ivalue> OP_SQRT
%token <tok.val.ivalue> OP_SQR
%token <tok.val.ivalue> OP_SIN
%token <tok.val.ivalue> OP_COS
%token <tok.val.ivalue> OP_TAN
%token <tok.val.ivalue> OP_ASIN
%token <tok.val.ivalue> OP_ACOS
%token <tok.val.ivalue> OP_ATAN
%token <tok.val.ivalue> POWER	
%token <tok.val.ivalue> SHIFTRIGHT
%token <tok.val.ivalue> SHIFTLEFT	
%token <tok.val.ivalue> AS_NORM		
%token <tok.val.ivalue> AS_ADD		
%token <tok.val.ivalue> AS_SUB		
%token <tok.val.ivalue> AS_OR		
%token <tok.val.ivalue> AS_XOR		
%token <tok.val.ivalue> AS_AND		
%token <tok.val.ivalue> AS_RIGHT	
%token <tok.val.ivalue> AS_LEFT	
%token <tok.val.ivalue> AS_MUL	
%token <tok.val.ivalue> AS_DIV	
%token <tok.val.ivalue> AS_MOD	
%token <tok.val.ivalue> LOR	
%token <tok.val.ivalue> LAND	
%token <tok.val.ivalue> TEST_EQ	
%token <tok.val.ivalue> TEST_NEQ
%token <tok.val.ivalue> REL_LT	
%token <tok.val.ivalue> REL_LTE	
%token <tok.val.ivalue> REL_GT 
%token <tok.val.ivalue> REL_GTE
%token <tok.val.ivalue> OP_FACT 
%token <tok.val.ivalue> AS_INC
%token <tok.val.ivalue> AS_DEC

%left ','
%right AS_NORM AS_ADD AS_SUB AS_OR AS_XOR AS_AND AS_RIGHT AS_LEFT AS_MUL AS_DIV AS_MOD
%right '?'
%left LOR
%left LAND
%left '|'
%left '^'
%left '&'
%left TEST_EQ TEST_NEW
%left REL_LT REL_LTE REL_GT REL_GTE
%left SHIFTRIGHT SHIFTLEFT
%left '+' '-'
%left '*' '/' '%'
%right POWER
%right UMINUS '!' '~'


%%

prog	:	exp END
			{ show_used_vars(format);
			  print($1,NOT_FORCED);
			  last_was_if = FALSE;
			}
	|	statement_list end
			{;}
	|	END
			{;}
	;


end	:	END
			{;}
	|
			{;}
	;


block	:	'{' 
			{ start_block();}
		statement_list '}'
			{ end_block();}
	|	'{' '}'
			{;}
	;


statement_list:	statement
			{;}
	|	statement_list statement
			{;}
	;


statement:
		block
			{;}
	|	exp ';'
			{ show_used_vars(format);
			  print($1,NOT_FORCED);
			  last_was_if = FALSE;
			}
	|	if_exp
			{;}
	|	else_exp
			{;}
	|	';'	
			{;}
	;

if_exp	:	IF 
			{ start_if();}
		'(' exp ')' 
			{ test_eval_flag($4);}
		statement 
			{ end_if();}
	;

else_exp:	ELSE
			{ start_else();}
		statement
			{ end_else();}
	;


exp	:	base 
			{ copy(&$$,$1);}
	|	exp ',' exp
			{ copy(&$$,$3);}
	|	VAR AS_NORM exp
			{ assign(&$$,$1,$3,AS_NORM);}
	|	VAR AS_ADD exp
			{ assign(&$$,$1,$3,AS_ADD);}
	|	VAR AS_SUB exp
			{ assign(&$$,$1,$3,AS_SUB);}
	|	VAR AS_OR exp
			{ assign(&$$,$1,$3,AS_OR);}
	|	VAR AS_XOR exp
			{ assign(&$$,$1,$3,AS_XOR);}
	|	VAR AS_AND exp
			{ assign(&$$,$1,$3,AS_AND);}
	|	VAR AS_RIGHT exp
			{ assign(&$$,$1,$3,AS_RIGHT);}
	|	VAR AS_LEFT exp
			{ assign(&$$,$1,$3,AS_LEFT);}
	|	VAR AS_MUL exp
			{ assign(&$$,$1,$3,AS_MUL);}
	|	VAR AS_DIV exp
			{ assign(&$$,$1,$3,AS_DIV);}
	|	VAR AS_MOD exp
			{ assign(&$$,$1,$3,AS_MOD);}
	|	exp QMARK 
			{ set_eval_cond_expr($1);} 
		exp ':' 
			{ toggle_eval_cond_expr();} 
		exp
			{ conditional(&$$,$1,$4,$7);}
	|	exp LOR exp
			{ lor(&$$,$1,$3);}
	|	exp LAND exp
			{ land(&$$,$1,$3);}
	|	exp '|' exp
			{ bor(&$$,$1,$3);}
	|	exp '^' exp
			{ bxor(&$$,$1,$3);}
	|	exp '&' exp
			{ band(&$$,$1,$3);}
	|	exp TEST_EQ exp
			{ test_eq(&$$,$1,$3);}
	|	exp TEST_NEQ exp
			{ test_neq(&$$,$1,$3);}
	|	exp REL_GT exp
			{ rel_gt(&$$,$1,$3);}
	|	exp REL_GTE exp
			{ rel_gte(&$$,$1,$3);}
	|	exp REL_LT exp
			{ rel_gte(&$$,$3,$1);}
	|	exp REL_LTE exp
			{ rel_gt(&$$,$3,$1);}
	|	exp SHIFTLEFT exp
			{ shiftleft(&$$,$1,$3);}
	|	exp SHIFTRIGHT exp
			{ shiftright(&$$,$1,$3);}
	|	exp '+' exp 
			{ add(&$$,$1,$3);}
	|	exp '-' exp
			{ subtract(&$$,$1,$3);}
	|	exp '*' exp
			{ multiply(&$$,$1,$3);}
	|	exp '/' exp
			{ divide(&$$,$1,$3);}
	|	exp '%' exp
			{ mod(&$$,$1,$3);}
	|	exp POWER exp
			{ power(&$$,$1,$3);}
	|	'-' exp	%prec UMINUS
			{ uminus(&$$,$2);}
	|	'~' exp
			{ ones_comp(&$$,$2);}
	|	'!' exp
			{ not(&$$,$2);}
	;


base 	:	DOUBLE
			{ copy(&$$,$1);}
	|	INT	
			{ copy(&$$,$1);}
	|	VAR
			{ get_val(&$$,$1);}
	|	AS_INC VAR
			{ pre_inc(&$$,$2);}
	|	VAR AS_INC
			{ post_inc(&$$,$1);}
	|	AS_DEC VAR
			{ pre_dec(&$$,$2);}
	|	VAR AS_DEC
			{ post_dec(&$$,$1);}
	|	OP_LOG '(' exp ')'
			{ op(&$$,$3,OP_LOG);}
	|	OP_LOG10 '(' exp ')'
			{ op(&$$,$3,OP_LOG10);}
	|	OP_LN '(' exp ')'
			{ op(&$$,$3,OP_LN);}
	|	OP_EXP '(' exp ')'
			{ op(&$$,$3,OP_EXP);}
	|	OP_SQRT '(' exp ')'
			{ op(&$$,$3,OP_SQRT);}
	|	OP_SQR '(' exp ')'
			{ op(&$$,$3,OP_SQR);}
	|	OP_SIN '(' exp ')'
			{ op(&$$,$3,OP_SIN);}
	|	OP_COS '(' exp ')'
			{ op(&$$,$3,OP_COS);}
	|	OP_TAN '(' exp ')'
			{ op(&$$,$3,OP_TAN);}
	|	OP_ASIN '(' exp ')'
			{ op(&$$,$3,OP_ASIN);}
	|	OP_ACOS '(' exp ')'
			{ op(&$$,$3,OP_ACOS);}
	|	OP_ATAN '(' exp ')'
			{ op(&$$,$3,OP_ATAN);}
	|	OP_FACT '(' exp ')'
			{ op(&$$,$3,OP_FACT);}
	|	'(' exp ')'
			{ copy(&$$,$2);}
	;

%%
