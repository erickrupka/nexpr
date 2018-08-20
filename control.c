#include <stdio.h>
#include "basics.h"
#include "nexpr_types.h"
#include "y.tab.h"
#include "error.h"
#include "main.h"

typedef struct STATE {
	int		eval_flag;
	int		last_was_if;
	int		last_if_taken;
	int		inside_block;
	struct STATE	*next_state_ptr;
} state_stack_type, *state_ptr_type;

state_ptr_type first_state = NULL;

int eval_on = TRUE;
int last_was_if = FALSE;
int last_if_taken = FALSE;
int inside_block = FALSE;

void push_state(void)
{
	state_ptr_type tmp;

	tmp = (state_ptr_type)MALLOC(sizeof(state_stack_type));
	if (tmp == NULL){
		error(NO_MEMORY,"");
	}
	tmp->eval_flag = eval_on;
	tmp->last_was_if = last_was_if;
	tmp->last_if_taken = last_if_taken;
	tmp->inside_block = inside_block;
	tmp->next_state_ptr = first_state;
	first_state = tmp;
/*
	printf("pushing state: eval_on      = %d\n",eval_on);
	printf("               last_was_if  = %d\n",last_was_if);
	printf("               last_if_taken= %d\n",last_if_taken);
	printf("               inside_block = %d\n",inside_block);
*/
}


void pop_state(void)
{
	state_ptr_type tmp;

	tmp = first_state;
	if (first_state != NULL){
		first_state = first_state->next_state_ptr;
		eval_on = tmp->eval_flag;
		last_was_if = tmp->last_was_if;
		last_if_taken = tmp->last_if_taken;
		inside_block = tmp->inside_block;
		free(tmp);
	}
/*
	printf("popped state: eval_on      = %d\n",eval_on);
	printf("              last_was_if  = %d\n",last_was_if);
	printf("              last_if_taken= %d\n",last_if_taken);
	printf("              inside_block = %d\n",inside_block);
*/
}

void set_eval_flag(void)
{
	eval_on = TRUE;
}

void clear_eval_flag(void)
{
	eval_on = FALSE;
}

void test_eval_flag(yytok_type tok)
{
	switch (tok.type){
		case INT:
		 	if (tok.val.ivalue){
				eval_on &= TRUE;
			}
			else {
				eval_on &= FALSE;
			}
			break;

		case DOUBLE: 
			if (tok.val.dvalue != 0.0){
				eval_on &= TRUE;
			}
			else {
				eval_on &= FALSE;
			}
			break;
		default: 
			error(BAD_IF_EXP,"");
			break;
	}
	last_if_taken = eval_on;
}

void check_if_if(void)
{
/*
	printf("checking if : eval_on      = %d\n",eval_on);
	printf("              last_was_if  = %d\n",last_was_if);
	printf("              last_if_taken= %d\n",last_if_taken);
	printf("              inside_block = %d\n",inside_block);
*/
	if (last_was_if){
		if (last_was_if && last_if_taken){
			eval_on = FALSE;
		}
		else {
			eval_on = TRUE;
		}
		return;
	}
	else {
		error(ELSE_NO_IF,"");
	}
}


void start_if(void)
{
	push_state();
	inside_block = TRUE;
}


void end_if(void)
{
	inside_block = FALSE;
	if (last_if_taken){
		pop_state();
		last_if_taken = TRUE;
	}
	else {
		pop_state();
		last_if_taken = TRUE;
	}
	last_was_if = TRUE;
}


void start_else(void)
{
	push_state();
	inside_block = TRUE;
	check_if_if();
}


void end_else(void)
{
	inside_block = FALSE;
	pop_state();
	last_was_if = FALSE;
}


void start_block(void)
{
	push_state();
	inside_block = TRUE;
}


void end_block(void)
{
	inside_block = FALSE;
	pop_state();
}
