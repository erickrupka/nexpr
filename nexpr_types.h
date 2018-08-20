/****************************************/
/*					*/
/*		nexpr_types.h		*/
/*					*/
/* definition of token type		*/
/*					*/
/****************************************/
typedef struct yystype {
	int	type;
	union {
		long	ivalue;
		double	dvalue;
	} val;
	char	string[80];
} yytok_type;

extern int pre_inc(yytok_type *to, yytok_type from);
extern int post_inc(yytok_type *to, yytok_type from);
extern int pre_dec(yytok_type *to, yytok_type from);
extern int post_dec(yytok_type *to, yytok_type from);

extern void yyerror(char *s);	// yacc
extern void reset_input(void);  // lex
