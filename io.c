#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "basics.h"
#include "main.h"
#include "error.h"
#include "strings.h"
#include "control.h"
#include "func.h"

typedef struct FILE_STACK {
	char			filename[100];
	long			offset;
	struct FILE_STACK	*next_file;	
} file_stack_type, *file_stack_ptr_type;

FILE	*init_file1;	/* $NEXPR_INIT file  */
FILE	*init_file2;	/* $HOME/.nexpr file */
FILE	*read_this;	/* file used for read file command */
int	read_file_1;	/* indicates if init_file1 has been read yet */
int	read_file_2;
int	reading_file = FALSE; /* indicated if gets should read from read_file */
char	filename[100];

file_stack_ptr_type first_file = NULL;

int 	verbose_mode = FALSE;
int 	initializing = FALSE;

FILE	*log_file;
char	log_file_name[100];

#define PRINT_BUF_LEN	256

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}


void clear_file_stack(void)
{
	file_stack_ptr_type file_ptr;
	file_stack_ptr_type next_file_ptr;

	file_ptr = first_file;
	while (file_ptr != NULL){
		next_file_ptr = file_ptr->next_file;
		free(file_ptr);
		file_ptr = next_file_ptr;	
	}	
	reading_file = FALSE;
	read_this = NULL;
	first_file = NULL;
}


int name_in_file_stack(char *name)
{
	file_stack_ptr_type	file_ptr;

	if (streq(filename,name)){
		return TRUE;
	}
	file_ptr = first_file;
	while (file_ptr != NULL){
		if (streq(file_ptr->filename,name)){
			return TRUE;
		}
		file_ptr = file_ptr->next_file;
	}
	return FALSE;
}


void push_file(void)
{
	file_stack_ptr_type stack_ptr;

	stack_ptr = (file_stack_ptr_type)MALLOC(sizeof(file_stack_type));
	strncpy(stack_ptr->filename,filename,100);
	stack_ptr->offset = ftell(read_this);
	stack_ptr->next_file = first_file;
	first_file = stack_ptr;
	fclose(read_this);
}


int pop_file(void)
{
	file_stack_ptr_type 	file_ptr;

	if (first_file != NULL){
		file_ptr = first_file;
		read_this = fopen(file_ptr->filename,"r");
		if (read_this == NULL){
			error(FILE_OPEN,file_ptr->filename);
		}
		if (fseek(read_this,file_ptr->offset,0) != 0){
			error(FILE_OPEN,file_ptr->filename);
		}
		reading_file = TRUE;
		strncpy(filename,file_ptr->filename,100);
		first_file = file_ptr->next_file;
		free(file_ptr);
		return TRUE;
	}
	return FALSE;
}

void dump_files(void)
{
	file_stack_ptr_type file_ptr;

	file_ptr = first_file;
	printf("file stack dump:\n");
	while (file_ptr != NULL){
		printf("	%s %ld\n",file_ptr->filename,file_ptr->offset);
		file_ptr = file_ptr->next_file;
	}
}


void verbose_on(int cmd_code, int obj_code, char *string)
{
	verbose_mode = TRUE;
}


void verbose_off(int cmd_code, int obj_code, char *string)
{
	verbose_mode = FALSE;
}


char GETCHAR(void)
{
	return getchar();
}


int PRINT_TO_BUF(va_list argp, char *buf, char *format, ...)
{
	char	*formatPtr;
	int	intVal;
	double	doubleVal;
	char	*s;
	char	tmpBuf[PRINT_BUF_LEN];
	char	printBuf[PRINT_BUF_LEN];
	int	numCharsPrinted;

	numCharsPrinted = 0;
	memset(tmpBuf,0,sizeof(tmpBuf));
	memset(printBuf,0,sizeof(tmpBuf));

	for (formatPtr=format; (*formatPtr != '\0') && (numCharsPrinted < PRINT_BUF_LEN-1); formatPtr++) {
		if (*formatPtr != '%') {
			printBuf[numCharsPrinted] = *formatPtr;
			numCharsPrinted++;
		}
		else {
			formatPtr++;
			switch (*formatPtr) {
				case 'c':
					intVal = va_arg(argp, int);
					printBuf[numCharsPrinted] = (char)(intVal & 0xff);
					numCharsPrinted++;
					break;
				case 'd':
					intVal = va_arg(argp, int);
					itoa(intVal, tmpBuf, 10);
					if ((strlen(tmpBuf) + numCharsPrinted) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					strcat(printBuf,tmpBuf);
					numCharsPrinted = strlen(printBuf);
					break;
				case 's':
					s = va_arg(argp, char *);
					if ((strlen(s) + numCharsPrinted) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					strcat(printBuf,s);
					numCharsPrinted = strlen(printBuf);
					break;
				case 'x':
					intVal = va_arg(argp, int);
					itoa(intVal, tmpBuf, 16);
					if ((strlen(tmpBuf) + numCharsPrinted) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					strcat(printBuf,tmpBuf);
					numCharsPrinted = strlen(printBuf);
					break;
				case 'o':
					intVal = va_arg(argp, int);
					itoa(intVal, tmpBuf, 0);
					if ((strlen(tmpBuf) + numCharsPrinted) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					strcat(printBuf,tmpBuf);
					numCharsPrinted = strlen(printBuf);
					break;
				case 'b':
					intVal = va_arg(argp, int);
					itoa(intVal, tmpBuf, 2);
					if ((strlen(tmpBuf) + numCharsPrinted) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					strcat(printBuf,s);
					numCharsPrinted = strlen(printBuf);
					break;
				case 'f':
					doubleVal = (double)va_arg(argp, double);
					sprintf(tmpBuf, "%lf", doubleVal);
					if ((strlen(tmpBuf) + numCharsPrinted) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					strcat(printBuf,tmpBuf);
					numCharsPrinted = strlen(printBuf);
					break;
				case '%':
					printBuf[numCharsPrinted] = '%';
					numCharsPrinted++;
					break;
				default:
					// invalid formatting char, print the % and the invalid char
					if ((strlen(tmpBuf) + 2) >= PRINT_BUF_LEN) {
						printBuf[numCharsPrinted] = '\0';
						break;
					}
					printBuf[numCharsPrinted] = '%';
					numCharsPrinted++;
					printBuf[numCharsPrinted] = *formatPtr;
					numCharsPrinted++;
			}
		}
	}

	printBuf[PRINT_BUF_LEN-1] = '\0';

	strcpy(buf, printBuf);
}



int PRINTF(char *format, ...)
{
	char	buf[PRINT_BUF_LEN];
	va_list	argp;

	va_start(argp,format);

	PRINT_TO_BUF(argp,buf,format);
	puts(buf);
	if (log_file != NULL){
		fputs(buf,log_file);
	}

	va_end(argp);
}


int FPRINTF(FILE *fp, char *format, ...)
{
	char	buf[PRINT_BUF_LEN];
	va_list	argp;

	va_start(argp,format);

	PRINT_TO_BUF(argp,buf,format);

	fputs(buf,fp);
	if (log_file != NULL){
		fputs(buf,log_file);
	}
}


char *strip_white(char *string)
{
	while (string && ((*string == '\t') || (*string == ' '))){
		string++;
	}
	return string;
}


char *GETS(char *string, int prompt)
{
	int read_from_file = FALSE;
	char *ret_val;

	initializing = FALSE;
	if (macro_running() && !code_is_running() && more_stack()){
		pop_stack();
	}
	else if (macro_running() && !code_is_running()){
		clear_macros();
		if (!verbose_mode){
			PRINTF("\n");
		}
	}
	if (macro_running() && code_is_running()){
		strcpy(string,code_running());
		if (verbose_mode == TRUE){
			PRINTF("%s> %s\n",prompt_string,string);
		}
		next_code();
		return strip_white(string);	
	}
	if (read_file_1 == FALSE){
		if (fgets(string,100,init_file1)){
			initializing = TRUE;
			if (string[strlen(string)-1] == '\n'){
				string[strlen(string)-1] = '\0';
			}
			if (verbose_mode == TRUE){
				PRINTF("%s> %s\n",prompt_string,string);
			}
			return strip_white(string);
		}
		else {
			read_file_1 = TRUE;
			fclose(init_file1);
		}
	}
	if (read_file_2 == FALSE){
		if (fgets(string,100,init_file2)){
			initializing = TRUE;
			if (string[strlen(string)-1] == '\n'){
				string[strlen(string)-1] = '\0';
			}
			if (verbose_mode == TRUE){
				PRINTF("%s> %s\n",prompt_string,string);
			}
			return strip_white(string);
		}
		else {
			read_file_2 = TRUE;
			fclose(init_file2);
		}
	}
	while (reading_file == TRUE){
		read_from_file = TRUE;
		if (fgets(string,100,read_this)){
			if (string[strlen(string)-1] == '\n'){
				string[strlen(string)-1] = '\0';
			}
			if (verbose_mode == TRUE){
				PRINTF("%s> %s\n",prompt_string,string);
			}
			return strip_white(string);
		}
		else {
			reading_file = FALSE;
			fclose(read_this);
			pop_file();
		}
	}
	if (!verbose_mode && (read_from_file == TRUE)){
		PRINTF("\n");
	}
/*
	printf("state: eval_on      = %d\n",eval_on);
	printf("       last_was_if  = %d\n",last_was_if);
	printf("       last_if_taken= %d\n",last_if_taken);
	printf("       inside_block = %d\n",inside_block);
*/
	if (prompt){
		update_vars();
		PRINTF("%s> ",prompt_string);
	}

	ret_val = fgets(cmd_line, sizeof(ret_val), stdin);

	if (log_file != NULL){
		fprintf(log_file,"%s\n",string);
	}

	if (ret_val == NULL){
		return NULL;
	}
	strip_white(cmd_line);
	return cmd_line;
}


void BPRINTF(long num)
{
	char temp[80];
	int i;

	sprintf(temp,"%o",(unsigned int)num);
	for (i=0; i<strlen(temp); i++){
		if (i == 0){
			switch(temp[i]){
				case '0': break;
				case '1': PRINTF("1");
					  break;
				case '2': PRINTF("10");
					  break;
				case '3': PRINTF("11");
					  break;
				case '4': PRINTF("100");
					  break;
				case '5': PRINTF("101");
					  break;
				case '6': PRINTF("110");
					  break;
				case '7': PRINTF("111");
					  break;
			}
		}
		else {
			switch(temp[i]){
				case '0': PRINTF("000");
					  break;
				case '1': PRINTF("001");
					  break;
				case '2': PRINTF("010");
					  break;
				case '3': PRINTF("011");
					  break;
				case '4': PRINTF("100");
					  break;
				case '5': PRINTF("101");
					  break;
				case '6': PRINTF("110");
					  break;
				case '7': PRINTF("111");
					  break;
			}
		}
	}
	PRINTF("\n");
}


void BFPRINTF(FILE *file, long num)
{
	char temp[80];
	int i;

	sprintf(temp,"%o",(unsigned int)num);
	for (i=0; i<strlen(temp); i++){
		if (i == 0){
			switch(temp[i]){
				case '0': break;
				case '1': FPRINTF(file,"1");
					  break;
				case '2': FPRINTF(file,"10");
					  break;
				case '3': FPRINTF(file,"11");
					  break;
				case '4': FPRINTF(file,"100");
					  break;
				case '5': FPRINTF(file,"101");
					  break;
				case '6': FPRINTF(file,"110");
					  break;
				case '7': FPRINTF(file,"111");
					  break;
			}
		}
		else {
			switch(temp[i]){
				case '0': FPRINTF(file,"000");
					  break;
				case '1': FPRINTF(file,"001");
					  break;
				case '2': FPRINTF(file,"010");
					  break;
				case '3': FPRINTF(file,"011");
					  break;
				case '4': FPRINTF(file,"100");
					  break;
				case '5': FPRINTF(file,"101");
					  break;
				case '6': FPRINTF(file,"110");
					  break;
				case '7': FPRINTF(file,"111");
					  break;
			}
		}
	}
	FPRINTF(file,"\n");
}


void init(void)
{
	char *filename;
	char *homepath;
	char filepath[100];

	read_file_1 = FALSE;
	read_file_2 = FALSE;
	filename = getenv("NEXPR_INIT");
	if (filename != NULL){
		init_file1 = fopen(filename,"r");
		if (init_file1 == NULL){
			read_file_1 = TRUE;
		}
	}
	else {
		read_file_1 = TRUE;
	}
	homepath = getenv("HOME");
	if (homepath != NULL){
		strcpy(filepath,homepath);
		strcat(filepath,"/.nexprc");
		init_file2 = fopen(filepath,"r");
		if (init_file2 == NULL){
			read_file_2 = TRUE;
		}
	}
	else {
		read_file_2 = TRUE;
	}
}


void read_file(int cmd_code, int obj_code, char *file_name)
{
	if (reading_file == TRUE){
		if (name_in_file_stack(file_name)){
			clear_file_stack();
			error(FILE_RECURSION,file_name);
		}
		push_file();
	}
	strncpy(filename,file_name,100);
	read_this = fopen(filename,"r");
	if (read_this == NULL){
		error(FILE_OPEN,filename);
	}
	reading_file = TRUE;
}


void log_off(int cmd_code, int obj_code, char *not_used)
{
	if (log_file != NULL){
		fclose(log_file);
	}
	else {
		error(LOGGING_OFF,"");
	}
	log_file = NULL;
	log_file_name[0] = '\0';
}


void log_on(int cmd_code, int obj_code, char *filename)
{
	if (log_file != NULL){
		error(LOGGING_ON,log_file_name);
	}
	if (filename[0] == '\0'){
		error(NO_FILE,"");
	}
	log_file = fopen(filename,"a");
	if (log_file == NULL){
		error(FILE_OPEN,filename);
	}
	strcpy(log_file_name,filename);
}
