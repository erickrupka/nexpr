#include <stdio.h>
#include <string.h>
#include "main.h"
#include "io.h"

char	formatchar = 'f';
char	format[20] = "%f";


int show_format(void)
{
	PRINTF("Current format is %s\n",format);
}


char format_type(char format[])
{
	int i;

	for (i=0; i<strlen(format); i++){
		switch (format[i]){
			case 'c':	formatchar = 'c';
					return format[i];
			case 'b': 	formatchar = 'b';
					return format[i];
			case 'd' :
			case 'u' :
			case 'o' :
			case 'x' :
			case 'X' :	formatchar = 'i';
					return format[i];
			case 'e' :
			case 'E' :
			case 'g' :
			case 'G' :
			case 'f' : 
					formatchar = 'f';
					return format[i];
		}
	}	
	return '\0';
}


void define_format(int cmd_code, int obj_code, char *format_arg)
{
	char *format_ptr;
	int   i;
	
	format_ptr = &format_arg[0];
	for (i=0; i<10; i++){
		if (*format_ptr == '\n'){
			*format_ptr = '\0';
			break;
		}
	}
	if (format_type(format_arg) != '\0')
		sprintf(format,"%%%s",format_arg);
	else {
		FPRINTF(stderr,"%s: invalid format, using %%f\n",prog_name);
		strcpy(format,"%f");
	}
}

