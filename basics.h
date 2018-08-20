
#define VERSION	"version 9.2 5/16/89"

/************************************************************************/
/*									*/
/* 9.2	 5/16/89	Fixed bug in handling of illegal command-line	*/
/*			options.					*/
/*									*/
/* 9.1	 5/ 2/89	Fixed bug in check for overflow check when      */
/*			evaluating -1**2				*/
/*									*/
/* 9.0	 5/ 2/89	Added "if", "else", "{", and "}".		*/
/*									*/
/* 8.1	 4/ 4/89	Changed "define format" to "format" command	*/
/*									*/
/* 8.0	 3/24/89	Added session logging.				*/
/*									*/
/* 7.0	 3/24/89	Can now send args to macros: $1 thru $9 in	*/
/*			macro definition get replaced with the $n'th	*/
/*			arg in the run macro command.			*/
/*									*/
/* 6.0	 3/24/89	Save command now implemented.			*/
/*									*/
/* 5.0	 3/23/89	Added ability to read commands/expressions from */
/*			a file.  A file can contain another read file   */
/*			command.  Recursion is checked and not allowed. */
/*			If a file reads another file, the first file    */
/*			reading continues after the second completes.	*/
/*									*/
/* 4.1	 3/20/89	Added check for ambigous command and object	*/
/*			substrings					*/
/*									*/
/* 4.0	 3/15/89	Added initialization files ./$NEXPR_INIT and	*/
/*			$HOME/.nexprc					*/
/*									*/
/* 3.3	 3/09/89	Added check for macro recursion	and resuming	*/
/*			a running macro after it calls another macro	*/
/*			and that macro ends.				*/
/*									*/
/* 3.2	 3/01/89	Added new table-driven command processor and	*/
/*			new commands.					*/
/*									*/
/* 3.1	 2/28/89	Fixed bug in check for overflow that caused	*/
/*			-1*-1 to get a log domain error and core dump.	*/
/*									*/
/* 3.0	 2/07/89	Added user-defined macros and fixed bug in	*/
/*			handling of whitespace				*/
/*									*/
/* 2.0	12/08/88	Added pre/post-inc/decrement, fixed bug in 	*/
/*			setting of conditional expression flag, fixed   */
/*			bug in handling of end-of-line			*/
/*									*/
/* 1.2	11/04/88	Fixed handling of conditional expressions	*/
/*									*/
/* 1.1	10/27/88	Fixed bug in calls to overflow()		*/
/*									*/
/* 1.0	10/25/88	created						*/
/*									*/
/************************************************************************/
/*									*/
/* enhancements:							*/
/*	for								*/
/*	while								*/
/*	do								*/
/*	functions							*/
/*	arrays								*/
/*	keystroke macros						*/
/*									*/
/************************************************************************/

#define TRUE	1
#define FALSE	0

#define streq(x,y)	(!strcmp(x,y))
#define strneq(x,y,z)	(!(strncmp(x,y,z)))

#define ABS(x)		((x) > 0 ? (x) : -(x))
#define MAX(x,y)	((x) > (y) ? (x) : (y))

extern void	free();
extern char	*strcpy();
extern void 	*malloc();
extern double	atof();
extern char 	*optarg;
extern int 	optind;
extern int 	opterr;
extern char	*getenv();
extern long	ftell();
extern int	fseek();
