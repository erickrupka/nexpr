#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "nexpr_types.h"
#include "y.tab.h"
#include "error.h"
#include "basics.h"
#include "main.h"
#include "format.h"
#include "io.h"
#include "func.h"
#include "control.h"
#include "strings.h"

extern int overflow(int op, yytok_type *x, yytok_type *y);


int eval_cond_expr;

int print(yytok_type token, int forced)
{
	if (!eval_on){
		return NO_ERROR;
	}
	/* if verbose mode is off, do not print unless forced to */
	if ((forced == NOT_FORCED) 
		&& (initializing || macro_running() || reading_file) 
		&& !verbose_mode){
		return NO_ERROR;
	}
	/* if evaluating a block do not print unless verbose is on */
	if (inside_block && !verbose_mode){
		return NO_ERROR;
	}
	if (error_result != NO_ERROR){
		token.type = INT;
		token.val.ivalue = 0;
	}

	switch (formatchar){

		case 'i':
			if (token.type == INT){
				PRINTF(format,token.val.ivalue);
			}
			else {
				PRINTF(format,dtoi(token.val.dvalue));
			}
			break;

		case 'f':
			if (token.type == INT){
				PRINTF(format,(double)token.val.ivalue);
			}
			else {
				PRINTF(format,token.val.dvalue);
			}
			break;

		case 'b':
			if (token.type == INT){
				BPRINTF(token.val.ivalue);
			}
			else {
				BPRINTF(dtoi(token.val.dvalue));
			}
			break;

		case 'c':
			if (token.type == INT){
				PRINTF(format,(char)token.val.ivalue);
			}
			else {
				PRINTF(format,(char)dtoi(token.val.dvalue));
			}
			break;

		default: PRINTF("can't print token");
			break;
	}
	PRINTF("\n");
	return NO_ERROR;
}


int pre_inc(yytok_type *to, yytok_type from)
{
	yytok_type one;

	one.type = INT;
	one.val.ivalue = 1;
	return assign(to,from,one,AS_ADD);
}


int post_inc(yytok_type *to, yytok_type from)
{
	yytok_type one,tmp;

	get_val(to,from);
	one.type = INT;
	one.val.ivalue = 1;
	return assign(&tmp,from,one,AS_INC);
}


int pre_dec(yytok_type *to, yytok_type from)
{
	yytok_type one;

	one.type = INT;
	one.val.ivalue = 1;
	return assign(to,from,one,AS_SUB);
}


int post_dec(yytok_type *to, yytok_type from)
{
	yytok_type one,tmp;

	get_val(to,from);
	one.type = INT;
	one.val.ivalue = 1;
	return assign(&tmp,from,one,AS_DEC);
}


int copy(yytok_type *to, yytok_type from)
{
	memcpy((char *)to,(char *)&from,sizeof(yytok_type));
	return NO_ERROR;
}


int add(yytok_type *sum, yytok_type a, yytok_type b)
{
	if (overflow((int)'+',&a,&b))
		return ERROR;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue + b.val.dvalue;
		}
		else if (b.type == INT){
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue + (double)b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->type = DOUBLE;
			sum->val.dvalue = (double)a.val.ivalue + b.val.dvalue;
		}
		else if (b.type == INT){
			sum->type = INT;
			sum->val.ivalue = a.val.ivalue + b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int subtract(yytok_type *sum, yytok_type a, yytok_type b)
{
	if (overflow((int)'-',&a,&b))
		return ERROR;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue - b.val.dvalue;
		}
		else if (b.type == INT){
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue - (double)b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->type = DOUBLE;
			sum->val.dvalue = (double)a.val.ivalue - b.val.dvalue;
		}
		else if (b.type == INT){
			sum->type = INT;
			sum->val.ivalue = a.val.ivalue - b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE, "Type mismatch");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int power(yytok_type *result, yytok_type a, yytok_type b)
{
	double atmp, btmp, rtmp;

	if (overflow(POWER,&a,&b))
		return ERROR;
	if (((a.type != INT) && (a.type != DOUBLE)) ||
		((b.type != INT) && (b.type != DOUBLE))){
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	else {
		if (a.type == INT){
			atmp = (double)a.val.ivalue;
		}
		else {
			atmp = a.val.dvalue;
		}
		if (b.type == INT){
			btmp = (double)b.val.ivalue;
		}
		else {
			btmp = b.val.dvalue;
		}
		if (atmp < 0.0){
			/* do extra checking for negative base */
			if (btmp != (double)dtoi(btmp)){
				/* fractional exponent, negative base */
				error(FRACT_POWER,"");
				return FRACT_POWER;
			}
			else {
				/* whole exponent */
				if (dtoi(btmp) & 1){
					/* odd exponent - negate result */
					/* must do this because log()   */
					/* doesn't handle negative nums */
					rtmp = -exp(btmp * log(-atmp));
				}
				else {
					/* even exponent - result ok */
					rtmp = exp(btmp * log(-atmp));
				}
			}
		}
		else {
			rtmp = exp(btmp * log(atmp));
		}
		if ((a.type == INT) && (b.type == INT) && 
			(rtmp <= max_int) &&
			((double)(dtoi(rtmp)) == rtmp)){
			result->type = INT;
			result->val.ivalue = dtoi(rtmp);
		}
		else {
			result->type = DOUBLE;
			result->val.dvalue = rtmp;
		}
	}
	return NO_ERROR;
}


int multiply(yytok_type *sum, yytok_type a, yytok_type b)
{
	if (overflow((int)'*',&a,&b))
		return ERROR;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue * b.val.dvalue;
		}
		else if (b.type == INT){
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue * (double)b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->type = DOUBLE;
			sum->val.dvalue = (double)a.val.ivalue * b.val.dvalue;
		}
		else if (b.type == INT){
			sum->type = INT;
			sum->val.ivalue = a.val.ivalue * b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE, "Type mismatch");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int divide(yytok_type *sum, yytok_type a, yytok_type b)
{
	long itmp;
	double dtmp;

	if (overflow((int)'/',&a,&b))
		return ERROR;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			if (b.val.dvalue == 0.0){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue / b.val.dvalue;
		}
		else if (b.type == INT){
			if (b.val.ivalue == 0){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->type = DOUBLE;
			sum->val.dvalue = a.val.dvalue / (double)b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			if (b.val.dvalue == 0.0){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->type = DOUBLE;
			sum->val.dvalue = (double)a.val.ivalue / b.val.dvalue;
		}
		else if (b.type == INT){
			if (b.val.ivalue == 0){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			itmp = a.val.ivalue / b.val.ivalue;
			dtmp = (double)a.val.ivalue / (double)b.val.ivalue;
			if ((double)itmp == dtmp){
				sum->type = INT;
				sum->val.ivalue = itmp;
			}
			else {
				sum->type = DOUBLE;
				sum->val.dvalue = dtmp;
			}
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE, "Type mismatch");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int mod(yytok_type *sum, yytok_type a, yytok_type b)
{
	long itmp;
	double dtmp;

	if (overflow((int)'%',&a,&b))
		return ERROR;
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			if ((b.val.dvalue == 0.0) || 
				(dtoi(b.val.dvalue) == 0.0)){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->val.ivalue = 
					dtoi(a.val.dvalue) % dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			if (b.val.ivalue == 0){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->val.ivalue = dtoi(a.val.dvalue) % b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			if ((b.val.dvalue == 0.0) 
				|| (dtoi(b.val.dvalue) == 0.0)){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->val.ivalue = a.val.ivalue % dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			if (b.val.ivalue == 0){
				error(DIV_BY_0,"");
				return DIV_BY_0;
			}
			sum->val.ivalue = a.val.ivalue % b.val.ivalue;
		}
		else {
			error(WRONG_TYPE, "Type mismatch");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE, "Type mismatch");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int uminus(yytok_type *to, yytok_type from)
{
	if ((from.type != DOUBLE) && (from.type != INT)){
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	to->type = from.type;
	if (to->type == DOUBLE)
		to->val.dvalue = -from.val.dvalue;
	else
		to->val.ivalue = -from.val.ivalue;
	return NO_ERROR;
}


int ones_comp(yytok_type *to, yytok_type from)
{
	if ((from.type != DOUBLE) && (from.type != INT)){
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	to->type = INT;
	if (to->type == DOUBLE)
		to->val.ivalue = ~dtoi(from.val.dvalue);
	else
		to->val.ivalue = ~from.val.ivalue;
	return NO_ERROR;
}


int not(yytok_type *to, yytok_type from)
{
	if ((from.type != DOUBLE) && (from.type != INT)){
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	to->type = INT;
	if (to->type == DOUBLE)
		to->val.ivalue = !dtoi(from.val.dvalue);
	else
		to->val.ivalue = !from.val.ivalue;
	return NO_ERROR;
}



int conditional(yytok_type *to, yytok_type a, yytok_type b, yytok_type c)
{
	if (((a.type != INT) && (a.type != DOUBLE)) ||
		((b.type != INT) && (b.type != DOUBLE)) ||
		((c.type != INT) && (c.type != DOUBLE))){
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	else if (a.type == INT){
		if (a.val.ivalue == 0){
			to->type = c.type;
			if (to->type == INT)
				to->val.ivalue = c.val.ivalue;
			else
				to->val.dvalue = c.val.dvalue;
		}
		else {
			to->type = b.type;
			if (to->type == INT)
				to->val.ivalue = b.val.ivalue;
			else
				to->val.dvalue = b.val.dvalue;
		}
	}
	else {
		if (a.val.ivalue == 0.0){
			to->type = c.type;
			if (to->type == INT)
				to->val.ivalue = c.val.ivalue;
			else
				to->val.dvalue = c.val.dvalue;
		}
		else {
			to->type = b.type;
			if (to->type == INT)
				to->val.ivalue = b.val.ivalue;
			else
				to->val.dvalue = b.val.dvalue;
		}
	}
	return NO_ERROR;
}


int lor(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			if ((a.val.dvalue != 0.0) || (b.val.dvalue != 0.0))
				sum->val.ivalue = 1;
			else
				sum->val.ivalue = 0;
		}
		else if (b.type == INT){
			if ((a.val.dvalue != 0.0) || (b.val.ivalue != 0))
				sum->val.ivalue = 1;
			else
				sum->val.ivalue = 0;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			if ((a.val.ivalue != 0) || (b.val.dvalue != 0.0))
				sum->val.ivalue = 1;
			else 
				sum->val.ivalue = 0;
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue || b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int land(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			if ((a.val.dvalue != 0.0) && (b.val.dvalue != 0.0))
				sum->val.ivalue = 1;
			else
				sum->val.ivalue = 0;
		}
		else if (b.type == INT){
			if ((a.val.dvalue != 0.0) && (b.val.ivalue != 0))
				sum->val.ivalue = 1;
			else
				sum->val.ivalue = 0;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			if ((a.val.ivalue != 0) && (b.val.dvalue != 0.0))
				sum->val.ivalue = 1;
			else 
				sum->val.ivalue = 0;
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue && b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int bor(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				dtoi(a.val.dvalue) | dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = dtoi(a.val.dvalue) | b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = a.val.ivalue | dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue | b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}
	

int bxor(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				dtoi(a.val.dvalue) ^ dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = dtoi(a.val.dvalue) ^ b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = a.val.ivalue ^ dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue ^ b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}
	


int band(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				dtoi(a.val.dvalue) & dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = dtoi(a.val.dvalue) & b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = a.val.ivalue & dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue & b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int test_eq(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = (a.val.dvalue == b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = 
				(a.val.dvalue == (double)b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				((double)a.val.ivalue == b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = (a.val.ivalue == b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int test_neq(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = (a.val.dvalue != b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = 
				(a.val.dvalue != (double)b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				((double)a.val.ivalue != b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = (a.val.ivalue != b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}


int rel_gt(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = (a.val.dvalue > b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = 
				(a.val.dvalue > (double)b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				((double)a.val.ivalue > b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = (a.val.ivalue > b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int rel_gte(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = (a.val.dvalue >= b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = 
				(a.val.dvalue >= (double)b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				((double)a.val.ivalue >= b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = (a.val.ivalue >= b.val.ivalue);
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int shiftleft(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				dtoi(a.val.dvalue) << dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = dtoi(a.val.dvalue) << b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = a.val.ivalue << dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue << b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int shiftright(yytok_type *sum, yytok_type a, yytok_type b)
{
	sum->type = INT;
	if (a.type == DOUBLE){
		if (b.type == DOUBLE){
			sum->val.ivalue = 
				dtoi(a.val.dvalue) >> dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = dtoi(a.val.dvalue) >> b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else if (a.type == INT){
		if (b.type == DOUBLE){
			sum->val.ivalue = a.val.ivalue >> dtoi(b.val.dvalue);
		}
		else if (b.type == INT){
			sum->val.ivalue = a.val.ivalue >> b.val.ivalue;
		}
		else {
			error(WRONG_TYPE,"");
			return WRONG_TYPE;
		}
	}	
	else {
		error(WRONG_TYPE,"");
		return WRONG_TYPE;
	}
	return NO_ERROR;
}



int factorial(yytok_type *result, double num)
{
	long tmp;

	result->type = DOUBLE;
	result->val.dvalue = num;
	if (overflow(OP_FACT,result,result))
		return ERROR;
	if (num < 0.0){
		error(NEG_FACT,"");
		return NEG_FACT;
	}
	else {
		tmp = dtoi(num);
		if (num != (double)tmp){
			error(FRACT_FACT,"");
			return FRACT_FACT;
		}
		else {
			result->type = INT;
			if ((tmp == 0 ) || (tmp == 1)){
				result->val.ivalue = 1;
			}
			else {
				result->val.ivalue = 1;
				while (tmp > 1){
					PRINTF("tmp = %d, result = %ld\n",
						tmp, result->val.ivalue);
					result->val.ivalue = 
						result->val.ivalue * tmp;
					tmp--;
				}
			}	
		}
	}
	return NO_ERROR;
}


int op(yytok_type *result, yytok_type from, int code)
{
	double dtmp;
	
	if (overflow(code,&from,&from))
		return ERROR;
	if ((from.type != INT) && (from.type != DOUBLE)){
		error(WRONG_TYPE, "Type mismatch");
		return WRONG_TYPE;
	}
	else if (from.type == INT){
		dtmp = (double)from.val.ivalue;
	}
	else{
		dtmp = from.val.dvalue;
	}
	result->type = DOUBLE;
	switch (code){
		case OP_FACT:
			return factorial(result,dtmp);
		case OP_LOG:
		case OP_LN:
			if (dtmp == 0.0){
				error(LOG_ZERO,"");
				return LOG_ZERO;
			}
			result->val.dvalue = log(dtmp);
			break;
		case OP_LOG10:
			if (dtmp == 0.0){
				error(LOG_ZERO,"");
				return LOG_ZERO;
			}
			result->val.dvalue = log10(dtmp);
			break;
		case OP_EXP:
			result->val.dvalue = exp(dtmp);
			break;
		case OP_SQRT:
			if (dtmp == 0.0){
				error(NEG_ROOT,"");
				return NEG_ROOT;
			}
			result->val.dvalue = sqrt(dtmp);
			break;
		case OP_SQR:
			if (from.type == INT){
				result->type = INT;
				result->val.ivalue = 
					from.val.ivalue * from.val.ivalue;
			}
			else {
				result->val.dvalue = dtmp * dtmp;
			}
			break;
		case OP_SIN:
			result->val.dvalue = sin(dtmp);
			break;
		case OP_COS:
			result->val.dvalue = cos(dtmp);
			break;
		case OP_TAN:
			result->val.dvalue = tan(dtmp);
			break;
		case OP_ASIN:
			if (dtmp > 1.0){
				error(ATRIG_RANGE,"");
				return ATRIG_RANGE;
			}
			result->val.dvalue = asin(dtmp);
			break;
		case OP_ACOS:
			if (dtmp > 1.0){
				error(ATRIG_RANGE,"");
				return ATRIG_RANGE;
			}
			result->val.dvalue = acos(dtmp);
			break;
		case OP_ATAN:
			result->val.dvalue = atan(dtmp);
			break;
	}
	return NO_ERROR;
}


void set_eval_cond_expr(yytok_type token)
{
	if ((token.type == DOUBLE) && (token.val.dvalue != 0.0)){
		eval_cond_expr = TRUE;
	}
	else if ((token.type == INT) && (token.val.ivalue != 0)){
		eval_cond_expr = TRUE;
	}
}


void clear_eval_cond_expr()
{
	eval_cond_expr = FALSE;
}


void toggle_eval_cond_expr()
{
	eval_cond_expr = !eval_cond_expr;
}


int eval_conditional()
{
	return eval_cond_expr;
}
