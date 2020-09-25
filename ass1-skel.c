/* Program to do "calculations" on numeric CSV data files.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   September 2020, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Enter your full name and student number here before submission]
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define MAXCOLS	20	/* maximum number of columns to be handled */
#define MAXROWS	999	/* maximum number of rows to be handled */
#define LABLEN  20	/* maximum length of each column header */
#define LINELEN 100	/* maximum length of command lines */

#define ERROR	(-1)	/* error return value from some functions */

#define O_NOC	'-'	/* the "do nothing" command */
#define O_IND	'i'	/* the "index" command */
#define O_ANA 	'a'	/* the "analyze" command */
#define O_DPY 	'd'	/* the "display" command */
#define O_PLT 	'p'	/* the "plot" command */
#define O_SRT 	's'	/* the "sort" command */

#define CH_COMMA ','	/* comma character */
#define CH_CR    '\r'	/* pesky CR character in DOS-format files */
#define CH_NL    '\n'	/* newline character */

/* if you wish to add further #defines, put them below this comment */


/* and then, here are some types for you to work with */
typedef char head_t[LABLEN+1];

typedef double csv_t[MAXROWS][MAXCOLS];

/****************************************************************/

/* function prototypes */

void get_csv_data(csv_t D, head_t H[],  int *dr,  int *dc, int argc,
	char *argv[]);
void error_and_exit(char *msg);
void print_prompt(void);
int  get_command(int dc, int *command, int ccols[], int *nccols);
void handle_command(int command, int ccols[], int nccols,
			csv_t D, head_t H[], int dr, int dc);
void do_index(csv_t D, head_t H[], int dr, int dc, int ccols[], int nccols);

/* add further function prototypes below here */


/****************************************************************/

/* main program controls all the action
*/
int
main(int argc, char *argv[]) {

	head_t H[MAXCOLS];	/* labels from the first row in csv file */
	csv_t D;		/* the csv data stored in a 2d matrix */
	int dr=0, dc=0;		/* number of rows and columns in csv file */
	int ccols[MAXCOLS];
	int nccols;
	int command;

	/* this next is a bit of magic code that you can ignore for
	   now, it reads csv data from a file named on the
	   commandline and saves it to D, H, dr, and dc
	   */
	get_csv_data(D, H, &dr, &dc, argc, argv);
	
	/* ok, all the input data has been read, safe now to start
	   processing commands against it */

	print_prompt();
	while (get_command(dc, &command, ccols, &nccols) != EOF) {
		handle_command(command, ccols, nccols,
			D, H, dr, dc);
		print_prompt();
	}

	/* all done, so pack up bat and ball and head home */
	printf("\nTa daa!!!\n");
	return 0;
}

/****************************************************************/

/* prints the prompt indicating ready for input
*/
void
print_prompt(void) {
	printf("> ");
}

/****************************************************************/

/* read a line of input into the array passed as argument
   returns false if there is no input available
   all whitespace characters are removed
   all arguments are checked for validity
   if no argumnets, the numbers 0..dc-1 are put into the array
*/
int
get_command(int dc, int *command, int columns[], int *nccols) {
	int i=0, c, col=0;
	char line[LINELEN];
	/* comand is in first character position */
	if ((*command=getchar()) == EOF) {
		return EOF;
	}
	/* and now collect the rest of the line, integer by integer,
	   sometimes in C you just have to do things the hard way */
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (isdigit(c)) {
			/* digit contributes to a number */
			line[i++] = c;
		} else if (i!=0)  {
			/* reached end of a number */
			line[i] = '\0';
			columns[col++] = atoi(line);
			/* reset, to collect next number */
			i = 0;
		} else {
			/* just discard it */
		}
	}
	if (i>0) {
		/* reached end of the final number in input line */
		line[i] = '\0';
		columns[col++] = atoi(line);
	}

	if (col==0) {
		/* no column numbers were provided, so generate them */
		for (i=0; i<dc; i++) {
			columns[i] = i;
		}
		*nccols = dc;
		return !EOF;
	}

	/* otherwise, check the one sthat were typed against dc,
	   the number of cols in the CSV data that was read */
	for (i=0; i<col; i++) {
		if (columns[i]<0 || columns[i]>=dc) {
			printf("%d is not between 0 and %d\n",
				columns[i], dc);
			/* and change to "do nothing" command */
			*command = O_NOC;
		}
	}
	/* all good */
	*nccols = col;
	return !EOF;
}

/****************************************************************/

/* this next is a bit of magic code that you can ignore for now
   and that will be covered later in the semester; it reads the
   input csv data from a file named on the commandline and saves
   it into an array of character strings (first line), and into a
   matrix of doubles (all other lines), using the types defined
   at the top of the program.  If you really do want to understand
   what is happening, you need to look at:
   -- The end of Chapter 7 for use of argc and argv
   -- Chapter 11 for file operations fopen(), and etc
*/
void
get_csv_data(csv_t D, head_t H[],  int *dr,  int *dc, int argc,
		char *argv[]) {
	FILE *fp;
	int rows=0, cols=0, c, len;
	double num;

	if (argc<2) {
		/* no filename specified */
		error_and_exit("no CSV file named on commandline");
	}
	if (argc>2) {
		/* confusion on command line */
		error_and_exit("too many arguments supplied");
	}
	if ((fp=fopen(argv[1], "r")) == NULL) {
		error_and_exit("cannot open CSV file");
	}

	/* ok, file exists and can be read, next up, first input
	   line will be all the headings, need to read them as
	   characters and build up the corresponding strings */
	len = 0;
	while ((c=fgetc(fp))!=EOF && (c!=CH_CR) && (c!=CH_NL)) {
		/* process one input character at a time */
		if (c==CH_COMMA) {
			/* previous heading is ended, close it off */
			H[cols][len] = '\0';
			/* and start a new heading */
			cols += 1;
			len = 0;
		} else {
			/* store the character */
			if (len==LABLEN) {
				error_and_exit("a csv heading is too long");
			}
			H[cols][len] = c;
			len++;
		}
	}
	/* and don't forget to close off the last string */
	H[cols][len] = '\0';
	*dc = cols+1;

	/* now to read all of the numbers in, assumption is that the input
	   data is properly formatted and error-free, and that every row
	   of data has a numeric value provided for every column */
	rows = cols = 0;
	while (fscanf(fp, "%lf", &num) == 1) {
		/* read a number, put it into the matrix */
		if (cols==*dc) {
			/* but first need to start a new row */
			cols = 0;
			rows += 1;
		}
		/* now ok to do the actual assignment... */
		D[rows][cols] = num;
		cols++;
		/* and consume the comma (or newline) that comes straight
		   after the number that was just read */
		fgetc(fp);
	}
	/* should be at last column of a row */
	if (cols != *dc) {
		error_and_exit("missing values in input");
	}
	/* and that's it, just a bit of tidying up required now  */
	*dr = rows+1;
	fclose(fp);
	printf("    csv data loaded from %s", argv[1]);
	printf(" (%d rows by %d cols)\n", *dr, *dc);
	return;
}
 
/****************************************************************/

void
error_and_exit(char *msg) {
	printf("Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

/****************************************************************/

/* the 'i' index command
*/
void
do_index(csv_t D, head_t H[], int dr, int dc,
		int ccols[], int nccols) {
	int i, c;
	printf("\n");
	for (i=0; i<nccols; i++) {
		c = ccols[i];
		printf("    column %2d: %s\n", c, H[c]);
	}
}


/*****************************************************************
******************************************************************

Below here is where you do most of your work, and it shouldn't be
necessary for you to make any major changes above this point (except
for function prototypes, and perhaps some new #defines).

Below this point you need to write new functions that provide the
required functionality, and modify function handle_command() as you
write (and test!) each one.

Tackle the stages one by one and you'll get there.

Have Fun!!!

******************************************************************
*****************************************************************/

/* this function examines each incoming command and decides what
   to do with it, kind of traffic control, deciding what gets
   called for each command, and which of the arguments it gets
*/
void
handle_command(int command, int ccols[], int nccols,
			csv_t D, head_t H[], int dr, int dc) { 
	if (command==O_NOC) {
		/* the null command, just do nothing */
	} else if (command==O_IND) {
		do_index(D, H, dr, dc, ccols, nccols);
/* you have to add more options here as you write further functions,
   for example...
	} else if (command==O_ANA) {
		do_analyze(D, H, dr, dc, ccols, nccols);
	} else if (command==O_DPY) {
		do_display(D, H, dr, dc, ccols, nccols);
*/
	/* and now a last option for things that aren't known */
	} else {
		printf("command '%c' is not recognized"
			" or not implemented yet\n", command);
	}
	return;
}

