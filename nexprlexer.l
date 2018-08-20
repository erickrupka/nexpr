%{

#include "nexpr_types.h"
#include "y.tab.h"
#include "main.h"
#include "error.h"
#include "io.h"
#include "control.h"

extern YYSTYPE yylval;

long bscanf(char *temp);

int char_ptr = 0;

char my_input(void);
#define YY_INPUT(buf,result,max_size) \
	       { \
	       int c = my_input(); \
	       result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
	       }

#define YY_NO_UNPUT

%}

bindigit	[01]
digit		[0-9]
hexdig		[0-9a-fA-F]
sign		[+-]
dot		\.
unsigned	{digit}+
unsignedhex	{hexdig}+
hex		0x{unsignedhex}
octal		0{unsigned}
binary		0b{bindigit}+
character	0c.
real		({unsigned}{dot}|{dot}{unsigned}|{unsigned}{dot}{unsigned})
exponent	[eE]{sign}?{unsigned}
float		({real}{exponent}|{unsigned}{exponent}|{real})
op_fact		(fact|factorial)
op_log		log
op_log10	log10
op_ln		ln
op_exp		exp
op_sqrt		sqrt
op_sqr		sqr
op_sin		sin
op_cos		cos
op_tan		tan
op_asin		asin
op_acos		acos
op_atan		atan
operator	[;,:{}+-/*^()\%|&~!]
variable	[A-Za-z][A-Za-z0-9_]*
whitespace	[ \t]

%%

\n		{
		 yylval.tok.type = END;
		 yylval.tok.string[0] = yytext[0];
		 yylval.tok.string[1] = '\0';
		 return END;
		}

{character}	{
		 yylval.tok.type = INT;
		 yylval.tok.val.ivalue = (long)yytext[2];
		 strcpy(yylval.tok.string,&yytext[2]);
	 	 return INT;
		}

{binary}	{
		 yylval.tok.type = INT;
		 yylval.tok.val.ivalue = bscanf(yytext);
		 strcpy(yylval.tok.string,&yytext[2]);
	 	 return INT;
		}

{octal}		{
		 yylval.tok.type = INT;
		 sscanf(yytext,"%lo",&yylval.tok.val.ivalue); 
		 strcpy(yylval.tok.string,&yytext[2]);
	 	 return INT;
		}

{unsigned}	{
		 yylval.tok.type = INT;
		 sscanf(yytext,"%ld",&yylval.tok.val.ivalue);
		 strcpy(yylval.tok.string,yytext);
		 return INT;
		}

{float}		{
		 yylval.tok.type = DOUBLE;
		 sscanf(yytext,"%lf",&yylval.tok.val.dvalue);
		 strcpy(yylval.tok.string,yytext);
		 return DOUBLE;
		}

{hex}		{
		 yylval.tok.type = INT;
	 	 sscanf(&yytext[2],"%lx",&yylval.tok.val.ivalue); 
		 strcpy(yylval.tok.string,&yytext[2]);
	 	 return INT;
		}

{operator}	{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return (int)yytext[0];
		}

\*\*		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return POWER;
		}

\>\>		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return SHIFTRIGHT;
		}

\<\<		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return SHIFTLEFT;
		}

\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_NORM;
		}

\>		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return REL_GT;
		}

\>\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return REL_GTE;
		}

\<		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return REL_LT;
		}

\<\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return REL_LTE;
		}

\=\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return TEST_EQ;
		}

\!\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return TEST_NEQ;
		}

\+\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_ADD;
		}

\-\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_SUB;
		}

\|\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_OR;
		}

\^\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_XOR;
		}

\&\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_AND;
		}

\|\|		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return LOR;
		}

\&\&		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return LAND;
		}

\>\>\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_RIGHT;
		}

\<\<\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_LEFT;
		}

\*\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_MUL;
		}

\/\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_DIV;
		}

\%\=		{
		 yylval.tok.type = STRING;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_MOD;
		}


\?		{
		 yylval.tok.type = QMARK;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return QMARK;
		}


\+\+		{
		 yylval.tok.type = AS_INC;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_INC;
		}


\-\-		{
		 yylval.tok.type = AS_DEC;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return AS_DEC;
		}


{op_fact}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_FACT;
		}

{op_log}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_LOG;
		}

{op_log10}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_LOG10;
		}

{op_ln}		{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_LN;
		}

{op_exp}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_EXP;
		}

{op_sqrt}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_SQRT;
		}

{op_sqr}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_SQR;
		}

{op_sin}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_SIN;
		}

{op_cos}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_COS;
		}

{op_tan}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_TAN;
		}

{op_asin}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_ASIN;
		}

{op_acos}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_ACOS;
		}

{op_atan}	{
		 yylval.tok.type = FUNCTION;
		 strcpy(yylval.tok.string,yytext);
	 	 yylval.tok.val.dvalue = 0.0;
		 return OP_ATAN;
		}

{whitespace}	{
		 /*
		 Do nothing.
		 */
		}

if		{
		 yylval.tok.type = IF;
		 yylval.tok.string[0] = '\0';
		 return IF;
		}

else		{
		 yylval.tok.type = ELSE;
		 yylval.tok.string[0] = '\0';
		 return ELSE;
		}


{variable}	{
		 yylval.tok.type = VAR;
		 strcpy(yylval.tok.string,yytext);
		 return VAR;
		}


.		{
		 yylval.tok.type = GARBAGE;
		 yylval.tok.string[0] = yytext[0];
		 yylval.tok.string[1] = '\0';
		 return GARBAGE;
		}

%%

int yywrap(void)
{
	return 1;
}


char my_input(void)
{
	char_ptr++;
	if ( ((cmd_line[char_ptr-1] == '\n') || (cmd_line[char_ptr-1] == '\0'))
		&& inside_block){
		strcpy(prompt_string,continue_prompt);
		if (GETS(cmd_line+1,PROMPT)){
			cmd_line[0] = ' ';
			char_ptr = 1;
		}
		else {
			error(ABANDON,"");
		}
		strcpy(prompt_string,prog_name);
	}		

	if (cmd_line[char_ptr-1] == '\0'){
		return EOF;
	}
	else {
		return cmd_line[char_ptr-1];
	}
}

//void my_unput(char c, char *unused)
void yyunput(char c, char *unused)
{
	if (char_ptr > 0) {
		char_ptr--;
	}
}

void reset_input(void)
{
	int i;

	for (i=0; i<80; i++)
		cmd_line[i] = '\0';
	char_ptr = 0;
}

long bscanf(char *temp)
{
	int i;
	int retval = 0;

	for (i=2; i<strlen(temp); i++){
		retval <<= 1;
		retval += temp[i] - (int)'0';
	}
	return retval;
}
