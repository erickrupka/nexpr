#define PROMPT		1
#define NO_PROMPT 	0

/* for print() - to force it to print if verbose_mode == FALSE */
#define FORCED		1
#define NOT_FORCED	0

extern char 	*GETS();
extern int 	PRINTF();
extern int 	FPRINTF();
extern char 	GETCHAR();
extern void	BPRINTF();
extern void	BFPRINTF();
extern void	init();
extern void	verbose_on();
extern void	verbose_off();
extern int	verbose_mode;
extern int	initializing;
extern void	read_file();
extern int	reading_file;
extern FILE	*read_this;
extern void	clear_file_stack();
extern void	log_on();
extern void	log_off();
