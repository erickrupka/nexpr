#include <stdio.h>
#include "basics.h"
#include "main.h"
#include "io.h"

int get_return()
{
	while (GETCHAR() != '\n'){
		;
	}
}

void help_general()
{
	PRINTF("\n\n");
	PRINTF("%s:\n",prog_name);
	PRINTF("	%s is a general purpose floating point calculator\n",
		prog_name);
	PRINTF("	that handles all arithmetic expressions allowed\n");
	PRINTF("	by the C programming language.	It supports variables,\n");
	PRINTF("	several different input and output formats, macros, and\n");
	PRINTF("	pre-defined constants.  To use it, simply type in\n");
	PRINTF("	the expression you wish to evaluate and hit return\n");
	PRINTF("	To quit, type \":q\"\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}



void help_expr()
{
	PRINTF("\n");
	PRINTF("	Expressions are composed of numbers, variables, and\n");
	PRINTF("	operators.  The rules for constructing expressions are\n");
	PRINTF("	the same as in C.  Pointer operations and arrays are\n");
	PRINTF("	not supported.\n");
	PRINTF("\n");
	PRINTF("	Division by zero and other errors are caught before\n");
	PRINTF("	they can do any damage.  If an error occurs, the current\n");
	PRINTF("	evaluation is terminated, an error message is printed,\n");
	PRINTF("	and zero is printed.\n");
	PRINTF("\n");
	PRINTF("	See the operators help page for information on operator\n");
	PRINTF("	precedence and guidance\n");
	PRINTF("\n");
	PRINTF("hit return to continue  ");
	get_return();
}


void help_iformat()
{
	PRINTF("\n\n");
	PRINTF("	For input, numbers can be specified in decimal\n");
	PRINTF("	<number>, hex 0x<number>, octal 0<number>, binary\n");
	PRINTF("	0b<number>, or character 0c<number>.  <Number> means\n");
	PRINTF("	0-9 in a decimal format, 0-F or 0-f in a hex format,\n");
	PRINTF("	or almost anything on the keyboard in a character format.\n");
	PRINTF("	Floating point numbers are also allowed.  They may\n");
	PRINTF("	have a mantissa containing an optional decimal point,\n");
	PRINTF("	and an upper- or lower-case 'e' followed by an\n");
	PRINTF("	optional '+' or '-' followed by a decimal number.\n");
	PRINTF("	For example, 12.34E+56 or .98e-7\n");
	PRINTF("	Floating point numbers are always base ten.\n");
	PRINTF("\n");
	PRINTF("hit return to continue  ");
	get_return();
}


void help_oformat()
{
	PRINTF("\n\n");
	PRINTF("	Output formatting is specified using a format\n");
	PRINTF("	character and an optional field specification.\n");
	PRINTF("	Most of the C formats are recognized (e.g. d,x,c,f)\n");
	PRINTF("	as well as 'b' for binary output.  The default format\n");
	PRINTF("	is f (floating point).\n");
	PRINTF("\n");
	PRINTF("	To change the output format, type\n");
	PRINTF("                :format <spec><format>\n");
	PRINTF("	where <format> is a C format character and <spec> is\n");
	PRINTF("	A field specification.  For example, \":format 5.2f\"\n");
	PRINTF("	will make all output print in floating point format in\n");
	PRINTF("	an area five spaces wide with two digits to the right\n");
	PRINTF("	of the decimal point.  See a C reference for more\n");
	PRINTF("	information on output formatting.  Also note that the\n");
	PRINTF("	\"b\" and \"c\" formats do not allow field specs.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}



void help_operators()
{
	PRINTF("\n");
	PRINTF("	The following operators are available.  They are \n");
	PRINTF("	listed in decreasing precedence.\n");
	PRINTF("\n");
	PRINTF("	The operators marked as \"integer\" will round their\n");
	PRINTF("	operands to the nearest integer before performing the\n");
	PRINTF("	operation.  Beware of using these operators on very\n");
	PRINTF("	large numbers.\n");
	PRINTF("\n");
	PRINTF("MORE - hit return to continue   ");
	get_return();
	PRINTF("\n");
	PRINTF("		++, --  - pre/post-inc/decrement x\n");
	PRINTF("		x**y	- x to the power of y\n");
	PRINTF("		!,-,~x	- logical not, unary -, and bitwise negation\n");
	PRINTF("			  Bitwise negation is integer.\n");
	PRINTF("		*, /, %	- multiplication, division, and\n");
	PRINTF("			  modulo (modulo is integer)\n");
	PRINTF("		+,-	- binary + and -\n");	
	PRINTF("		>>, <<	- Bit shifts (integer)\n");
	PRINTF("		<, >,\n");
	PRINTF("		<=, =>	- Relational tests\n");
	PRINTF("		==, !=	- Relational tests\n");
	PRINTF("		&	- Bitwise and (integer)\n");
	PRINTF("		^	- Bitwise xor (integer)\n");
	PRINTF("		|	- Bitwise or (integer)\n");
	PRINTF("		&&	- Logical and (integer)\n");
	PRINTF("		||	- Logical or (integer)\n");
	PRINTF("		?:	- Conditional (integer)\n");
	PRINTF("\n");
	PRINTF("MORE - hit return to continue  ");
	get_return();
	PRINTF("\n");
	PRINTF("		=,   +=,\n");
	PRINTF("		-=,  ^=,\n");
	PRINTF("		|=,  &=,\n");
	PRINTF("		>>=, <<=,\n");
	PRINTF("		*=,  /=,\n");
	PRINTF("		%=/	- Assignment operators (^=,|=,&=,\n");
	PRINTF("			  >>=,<<=,%= are integer)\n");
	PRINTF("\n");
	PRINTF("hit return to continue  ");
	get_return();
}
	

void help_variables()
{
	PRINTF("\n\n");
	PRINTF("	Variables must start with a letter [A-Za-z] and\n");
	PRINTF("	may contain letters, numbers, and underscores (_)\n");
	PRINTF("	Variables are \"declared\" when they are assigned a\n");
	PRINTF("	value.  All variables are double precision.\n");
	PRINTF("\n");
	PRINTF("	If a variable is assigned a value in a expression,\n");
	PRINTF("	the value of the variable will be printed before the\n");
	PRINTF("	value of the expression.  Variables retain their\n");
	PRINTF("	values between	calculations.\n");	
	PRINTF("\n");
	PRINTF("	To see all the variables and their values, type\n");
	PRINTF("	\":show variable\" at the prompt.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}


void help_commands()
{
	PRINTF("\n\n");
	PRINTF("        Commands are invoked by typing a colon followed by\n");
	PRINTF("        a command, type, and instance.  The type and instance\n");
	PRINTF("        are required for some commands and optional for others\n");
	PRINTF("        Commands are:\n");
	PRINTF("                define       - define something\n");
	PRINTF("                show         - show something\n");
	PRINTF("                clear        - clear something\n");
	PRINTF("                run          - run a macro\n");
	PRINTF("                !            - shell escape\n");
	PRINTF("                quit or exit - quit\n");
	PRINTF("                help         - give this help\n");
	PRINTF("                save         - save something to a file\n");
	PRINTF("                log          - turn session logging on or off\n");
	PRINTF("                debug        - not useful\n");
	PRINTF("                verbose      - verbose execution of commands\n");
	PRINTF("                               on or off\n");
	PRINTF("\n\n");
	PRINTF("MORE - hit return to continue  ");
	get_return();
	PRINTF("\n\n");
	PRINTF("        Types modify the command.  For instance, the command\n");
	PRINTF("        \"define\" can be applied to types \"format\",\n");
	PRINTF("        \"macro\", etc.\n");
	PRINTF("        Types are:\n");
	PRINTF("                variable\n");
	PRINTF("                function\n");
	PRINTF("                macro\n");
	PRINTF("                format\n");
	PRINTF("                version\n");
	PRINTF("                on\n");
	PRINTF("                off\n");
	PRINTF("\n");
	PRINTF("        The function type has not yet been implemented.\n");
	PRINTF("\n\n");
	PRINTF("MORE - hit return to continue  ");
	get_return();
	PRINTF("\n\n");
	PRINTF("        Instances are individual variables, macros, etc.\n");
	PRINTF("        Some commands/types need instances, some don't.\n");
	PRINTF("        Some will take an instance but will also work without\n");
	PRINTF("        one.  For example, the \"quit\" command does not need\n");
	PRINTF("        a type and it doesn't need an instance.  The \"show\"\n");
	PRINTF("        command needs a type but does not need an instance,\n");
	PRINTF("        although it will accept one.  If an instance is given,\n");
	PRINTF("        just the value of that instance will be shown.  If\n");
	PRINTF("        you were to type \":show variable xyz\", the value\n");
	PRINTF("        of xyz would be printed.  If you were to type \":show\n");
	PRINTF("        variable\", the names and values of all variables\n");
	PRINTF("        would be shown.\n");
	PRINTF("\n\n");
	PRINTF("MORE - hit return to continue  ");
	get_return();
	PRINTF("\n\n");
	PRINTF("        To save on typing, abbreviations for commands and\n");
	PRINTF("        types are accepted.  \":sh v xyz\" is ok.  Be careful\n");
	PRINTF("        with ambiguous abbreviations - \":s v xyz\" might\n");
	PRINTF("        save xyz instead of showing it!\n");
	PRINTF("\n");
	PRINTF("        If you are not sure whether or not a command will work\n");
	PRINTF("        with a type, or a type with an instance, try it.\n");
	PRINTF("        If it cannot be done, an error message will appear\n");
	PRINTF("        and no damage will be done.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}



void help_funcs()
{
	PRINTF("\n\n");
	PRINTF("	There are several built-in functions.  All take\n");
	PRINTF("	one expression as an argument.  The functions are:\n");
	PRINTF("		log()   - natural logarithm\n");
	PRINTF("		ln()    - natural logarithm\n");
	PRINTF("		log10() - log base 10\n");
	PRINTF("		exp()   - raise e to the power...\n");
	PRINTF("		sqrt()  - square root\n");
	PRINTF("		sqr()   - square\n");
	PRINTF("		sin()   - sine\n");
	PRINTF("		cos()   - cosine\n");
	PRINTF("		tan()	- tangent\n");
	PRINTF("		asin()  - arcsine\n");
	PRINTF("		acos()  - arccosine\n");
	PRINTF("		atan()  - arctangent\n");
	PRINTF("		fact()  - factorial\n");
	PRINTF("\n");
	PRINTF("	All trigonometric functions assume their arguments\n");
	PRINTF("	are in radians.  To convert radians to degrees, \n");
	PRINTF("	multiply the radians by pi/180.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}


void help_keywords()
{
	PRINTF("\n\n");
	PRINTF("	Several variable names are reserved and may not\n");
	PRINTF("	be used to store values.  They are as follows.\n");
	PRINTF("		e, E         - 2.718282\n");
	PRINTF("		pi, PI       - 3.141593\n");
	PRINTF("		true, TRUE   - 1\n");
	PRINTF("		false, FALSE - 0\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}

void help_macros()
{
	PRINTF("\n\n");
	PRINTF("        Macros can be defined and used as an easy way\n");
	PRINTF("        of repeating any number of calculations.  They\n");
	PRINTF("        may do anything you can do yourself, including\n");
	PRINTF("        executing other macros.  Macros cannot execute\n");
	PRINTF("        themselves directly or indirectly as there is\n");
	PRINTF("        presently no way to control the flow of execution\n");
	PRINTF("        and therefore no way of terminating the recursion.\n");
	PRINTF("\n");
	PRINTF("        To define a macro, use the command \":define macro\n");
	PRINTF("        some_name\".  You will be prompted to enter the\n");
	PRINTF("        commands that the macro will execute with the \"define>\"\n");
	PRINTF("        prompt.  Enter any expression you like.  After you\n");
	PRINTF("        have entered the expression (or command), hit return.\n");
	PRINTF("        The \"define >\" prompt will show up again and you\n");
	PRINTF("        may enter another expression or command.  When you are\n");
	PRINTF("        done, type \"end\" and you will be returned to the %s\n",
		prog_name);
	PRINTF("        prompt.  To run the macro, type \":run some_name\"\n");
	PRINTF("\n");
	PRINTF("        If a macro executes another macro, the executer will\n");
	PRINTF("        will resume after the executee finished.\n");
	PRINTF("\n\n");
	PRINTF("MORE - hit return to continue\n");
	PRINTF("\n\n");
	PRINTF("        Macros can have up to nine arguments.  To use arguments\n");
	PRINTF("        use \"$1\" through \"$9\" in the macro definition.\n");
	PRINTF("        When you run the macro, simply type the values you\n");
	PRINTF("        want to replace the arguments after the macro name.\n");
	PRINTF("        For example:\n");
	PRINTF("            :define macro abc\n");
	PRINTF("            $1 = $2 + $3\n");
	PRINTF("            end\n");
	PRINTF("\n");
	PRINTF("            :run macro abc a b c\n");
	PRINTF("\n");
	PRINTF("        Will result in the expression \"a = b + c\" getting\n");
	PRINTF("        evaluated.\n");
	PRINTF("\n");
	PRINTF("        If too few arguments are given in the :run command,\n");
	PRINTF("        %s will complain and not execute the macro.\n",prog_name);
	PRINTF("        If too many arguments are given, the extra ones are\n");
	PRINTF("        ignored and no warning is printed.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}

void help_files()
{
	PRINTF("\n\n");
	PRINTF("        File reading is supported by %s.  The contents\n",
		prog_name);
	PRINTF("        of a file is processed just as if you had typed\n");
	PRINTF("        it to the %s prompt.  Files can contain all\n",
		prog_name);
	PRINTF("        commands and expressions.  They can even read\n");
	PRINTF("        other files.  When a file is read, it acts just\n");
	PRINTF("        like a macro.\n");
	PRINTF("\n");
	PRINTF("        To read a file type \":read <filename>\", where\n");
	PRINTF("        <filename> is the name of the file you want to\n");
	PRINTF("        read.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}


void help_save()
{
	PRINTF("\n\n");
	PRINTF("        If you wish to record the state of your session\n");
	PRINTF("        with %s, you can do so using the save command.\n",
		prog_name);
	PRINTF("        To use it do \":save <file_name>\" where <file_name>\n");
	PRINTF("        is the name of the file you want to save to.  All\n");
	PRINTF("        variables and macros are saved such that you can\n");
	PRINTF("        read the file back in and have all variables and\n");
	PRINTF("        macros as they were when you saved them.\n");
	PRINTF("\n");
	PRINTF("        You can also save individual variables and macros\n");
	PRINTF("        by giving the name of the variable or macro you\n");
	PRINTF("        want to save after the file name.\n");
	PRINTF("               :save file_abc any_macro_name\n");
	PRINTF("\n");
	PRINTF("        Everything saved is appended to the file.  If the\n");
	PRINTF("        file does not exist, it is created.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}

void help_log()
{
	PRINTF("\n\n");
	PRINTF("        You can save everything you see on the screen to\n");
	PRINTF("        a file with the log command.  To turn on logging\n");
	PRINTF("        do \":log on <file_name>\" where <file_name> is the\n");
	PRINTF("        name of the file you want to send the output to.\n");
	PRINTF("        To turn off logging do \":log off\"\n");
	PRINTF("\n");
	PRINTF("        All output while logging is in effect is appended\n");
	PRINTF("        to the given file.  If the file does not exist it\n");
	PRINTF("        will be created.\n");
	PRINTF("\n");
	PRINTF("        You can log to only one file at a time.\n");
	PRINTF("\n\n");
	PRINTF("hit return to continue  ");
	get_return();
}


void help()
{
	int inchar = 0;
	int res;
	char in[80];

	while (TRUE){
		do {
			PRINTF("\n\n");
			PRINTF("        What kind of help do you want?\n");
			PRINTF("\n");
			PRINTF("                 1. General\n");
			PRINTF("                 2. Expressions\n");
			PRINTF("                 3. Operators\n");
			PRINTF("                 4. Variables\n");
			PRINTF("                 5. Built-in Functions\n");
			PRINTF("                 6. Input Formatting\n");
			PRINTF("                 7. Output Formatting\n");
			PRINTF("                 8. Commands\n");
			PRINTF("                 9. Keywords / Constants\n");
			PRINTF("                10. Macros\n");
			PRINTF("                11. Reading Files\n");
			PRINTF("                12. Session logging\n");
			PRINTF("                13. Saving things\n");
			PRINTF("                14. Quit help\n");
			PRINTF("\n");
			PRINTF("        Enter number of selection: ");
			GETS(in,NO_PROMPT);
			res = sscanf(in,"%d",&inchar);
			if (!res){
				PRINTF("bad input, hit return to continue\n");
				GETS(in,NO_PROMPT);
			}
		} while (!res);
		switch(inchar){
			case 1:	help_general();
				break;
			case 2:	help_expr();
				break;
			case 3:	help_operators();
				break;
			case 4: help_variables();
				break;
			case 5:	help_funcs();
				break;
			case 6:	help_iformat();
				break;
			case 7:	help_oformat();
				break;
			case 8:	help_commands();
				break;
			case 9:	help_keywords();
				break;
			case 10: help_macros();
				break;
			case 11: help_files();
				break;
			case 12: help_log();
				break;
			case 13: help_save();
				break;
			default: return;
		}
	}
}
		
		
