/*
 * Copyright (c) 2018 Oleg Pyzin. All rights reserved.
 *-------------------------------------+
 * wcn.c -> word count new             |
 * wcn.exe  -> *NIX style command line |
 * wcnv.exe -> VMS  style command line |
 *-------------------------------------+
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author may be used to endorse or promote 
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#ifdef VMS
#include <file.h>
#else

#endif

/* To parse system errors */
#include <errno.h>

/* Uncomment this if use iswspase()
#include <wchar.h>
#include <wctype.h>
*/

/* Project include */
#include "config.h"

/*--------------------------------------------
    Define variable
---------------------------------------------*/
static int errors;

/*
  Declare options:
  -c -> optchar;
  -m -> optmchar multibyte characters;
  -w -> optword;
  -l -> optline;
  -L -> optlline is longest line in a file(files);
  -t -> counters of each character in the array.
*/
static int optchar, optmchar, optword, optline, optlline, opttable;
static unsigned int cntchar, cntword, cntline, cntlline;
static unsigned int cntchar_ascii[256];


#define ILLEGAL_OPTIONS     5
#define LOST_OPTION_SYMBOL  6
#define COUNTER_OVERFLOW   10
/* Trigger ON/OFF   */
#define ON		    1
#define OFF		    0

/*--------------------------------------------
    Define function
---------------------------------------------*/
void show_table( unsigned int[] );     /* this function in the file table.c */

#ifdef VMS_STYLE
/*--------------------------------------------------------------------*/
unsigned char parse_cmd( int cntopt, char *opt[] )
{
	unsigned char choice=0;
	char *p;

	p = opt[cntopt];
	if( *p == '/' )
	  {
		choice = *(p+1);
                if( !choice )
                        errors = LOST_OPTION_SYMBOL;

          }
        /* Return ZERO if no option     */
        /* But if lost symbol after [-] */
        /* set error and                */
        /* return a zero                */

	return choice;
}

#else /* #ifdef VMS_STYLE */
/*--------------------------------------------------------------------*/
unsigned char parse_cmd(int cntopt, char *opt[])
{
	unsigned char choice=0;
	char *p;


        p = opt[cntopt];

	if( *p == '-' )
	  {
		choice =(unsigned char) *(p+1);
		if( !choice )
			errors = LOST_OPTION_SYMBOL;
          }
        /* Return ZERO if no option     */
	/* But if lost symbol after [-] */
        /* set error and                */
	/* return a zero                */

	return choice;
}

#endif  /* #ifdef VMS_STYLE */

/*---------------------------------------------------------------------*/
void show_errors( int err )
{
	printf("ERROR occured:\n");
	printf("-----------------------------------------------------\n");
	switch( err )
	{
		case ILLEGAL_OPTIONS :
			printf("Illegal option(s) in the command line.\n");
			break;
		case LOST_OPTION_SYMBOL :
			printf("Lost option symbol.\n");
			break;
		case 1 :
			printf("Error open file. Check if file exist.\n");
			break;
		case 2 :
			printf("Error reading data from file.\n");
			break;
		case COUNTER_OVERFLOW :
			printf("Counter of CHAR OVERFLOW.\n");
			break;
		default :
			printf("Error number [%d].\n", err);
                        break;
	}

	/* Check and parse GLOBAL errno variable */
	/* Thinking about it
	<errno.h>
	print errors
	------------
	errno
	-> check EVMSERR and print error perror();
	*/	
	
	printf("\n\n");
}

#ifdef VMS_STYLE
/*--------------------------------------------------------------------*/
void usage()
{
        printf("USAGE:\n");
        printf("-----------------------------------------------------\n");
        printf("wc [/Lcwlmt] [filename1 filename2 ...]\n");
        printf("------------\n");
        printf("Where:\n");
        printf("  /c Counting characters in the file(files);\n");
        printf("  /w Counting words in file(files);\n");
        printf("  /l Counting lines in the file(files);\n");
        printf("  /L Find longest line in a file or a list of files;\n");
        printf("  /t Print characters table.\n");
}

#else  /* #ifdef VMS_STYLE */
/*--------------------------------------------------------------------*/
void usage()
{
	printf("USAGE:\n");
	printf("-----------------------------------------------------\n");
	printf("wc [-Lcwlmt] [filename1 filename2 ...]\n");
	printf("------------\n");
	printf("Where:\n");
	printf("  -c Counting characters in the file(files);\n");
	printf("  -w Counting words in file(files);\n");
	printf("  -l Counting lines in the file(files);\n");
	printf("  -L Find longest line in a file or a list of files;\n");
	printf("  -t Print characters table.\n");
} 
#endif  /* #ifdef VMS_STYLE */


/*---------------------------------------------------------------------*/
#define MAXBUFF 512

int cnt( char *filename )
{
	int fdescr;
	int len;
	int trigger;
	int tmplline;
	unsigned int i;
	unsigned char input[MAXBUFF], *ptr;

	/* Initialize variable */


#ifdef DEBUG
	printf("Filename to processing is [%s].\n", filename );
#endif	
        /* Open a file with read only
	 * with user default file protection the 3-d arg "0"
	 * without any specific VMS options i.e.
	 * like *NIX
	 */
#ifdef VMS
	fdescr = open( filename, O_RDONLY, 0 );
#else
        if( filename )  fdescr = open( filename, O_RDONLY, 0 );
        else            fdescr = STDIN_FILENO;
#endif
        if( fdescr < 0 )
		return 1;
#ifdef DEBUG
	printf("Open file is [%s].\n", filename );
#endif
	/* Set trigger ON */
        trigger = ON;
	tmplline = 0;
	do {
		len = read( fdescr, input, MAXBUFF);
		/* Check is it error */
		if( len == -1 )
			return 2;
		for( i=0, ptr=input; i<len; i++, ptr++ )
		    {
			if( cntchar == 0xFFFFFFFF )
				return COUNTER_OVERFLOW;
			cntchar++;		/* ++ counters of a characters       */
			cntchar_ascii[*ptr]++;	/* ++ counters of a single character */
			tmplline++;		/* ++ temp count of lenght of line   */
			if( *ptr == '\n' )
			   {
				cntline++;	/* if new line ++ counters of a line */
				if( tmplline > cntlline )
				   cntlline = tmplline;
				tmplline = 0;
			   }
			/*
			 * To count words using a trigger
			 * when begin space trigger ON
			 * when space ends trigger OFF and cntword++
			 */
			if( optword )
			   {
			     if( isspace( *ptr ) )
				    trigger = ON;
			     else if( trigger )					
				  {
				    trigger = OFF;
				    cntword++;
				  }
			   }
		    }
	} while( len );	 

	if( fdescr )
	   {
#ifdef DEBUG
		printf("Close a file to processing [%s].\n", filename );
#endif
		return close( fdescr ); 
	   }
	return 0;
}


/*---------------------------------------------------------------------*/
void show_cnt(  char *fl,
		unsigned int ichar,
		unsigned int iword,
		unsigned int iline,
		unsigned int ichar_ascii[] )
{

	if( opttable )
                show_table( ichar_ascii );
	if( optline )
		printf(" %10d", iline );
	if( optword )
		printf(" %10d", iword );
	if( optchar )
		printf(" %10d", ichar );

	printf("  %s\n", fl );
}


/*---------------------------------------------------------------------*/
int main( int argc,  char *argv[] )
{
	int i, total;
        unsigned char chopt;
	int listFiles;
	int cmdhelp;
	/* For the total counters */
	unsigned int cntchar_t, cntword_t;
	unsigned int cntline_t, cntlline_t;
	unsigned int cntchar_ascii_t[256];
#ifdef VMS
	char *flname = "SYS$INPUT";
#else
	char *flname = NULL;
#endif
	/* errors is a global variable */
	errors = 0;


	/* Initialize options variable */
	optline = optword = optchar = optmchar = optlline = opttable = 0;
	/* Option for the HELP */
	cmdhelp = 0;

        /* The first argument from list of command line arguments */
	i = 1;

	while( i<argc && !cmdhelp && !errors && (chopt=parse_cmd(i,argv)) )
	{
		switch(chopt)
		{
		case 'l':
			optline = 1;
			break;
		case 'w':
			optword = 1;
			break;
		case 'c':
			optchar = 1;
			break;
		case 'L':
			optlline = 1;
			break;
		case 'm':
#ifdef DEBUG
			printf("Option [-m]. Thinking about How do I make it.\n");
#endif
			break;
		case 't':
			opttable = 1;
			break;
		case 'h':
			cmdhelp = 1;
			break;
		default :
			errors = ILLEGAL_OPTIONS;
			break;
		}
	  ++i;
	}

	/* Keep a pozition of list of files */
        listFiles = i;
	/* how many files are there */
	total = 0;

	/* Check do we have an error after parsed command line */
	/* or we have an option HELP                           */
	/* The first show errors if exist                      */
        /* and always show USAGE                               */
		
        if( cmdhelp || errors )
	{
		if( errors ) show_errors( errors );
		usage();
		return errors;
	}

	/*-------------------------------------*/
	/*        Initialize variable          */
	for( i=0; i<256; i++ )
	   {
		cntchar_ascii[i]   = 0;
		cntchar_ascii_t[i] = 0;
	   }
	cntchar = cntword = cntline = 0;
	cntchar_t = cntword_t = cntline_t = 0;

#ifdef DEBUG
        /* show setted options */
	if( optlline )
		printf("Setted option LongLine count.\n");
	if( optline )
		printf("Setted option Line count.\n");
	if( optword )
		printf("Setted option Word count.\n");
	if( optchar )
		printf("Setted option Char count.\n");
	if( opttable )
		printf("Setted option Table character print.\n");
#endif
	if( listFiles < argc )
	   {
		total = argc - listFiles;

		while( listFiles < argc )
		    {
			errors = cnt( argv[listFiles] );
			if( errors )
				show_errors( errors );
			else
			   {
				show_cnt( argv[listFiles],
					  cntchar, cntword, cntline, cntchar_ascii );
				if( total > 1 )
				   {
					/* save counters */
					if( (0xFFFFFFFF - cntchar) < cntchar_t )
					   {
						show_errors( COUNTER_OVERFLOW );
						return COUNTER_OVERFLOW;
					   }
					cntchar_t += cntchar;
					for( i=0; i<256; i++)
						cntchar_ascii_t[i] += cntchar_ascii[i];
					cntword_t += cntword;
					cntline_t += cntline;
					if( cntlline > cntlline_t ) 
						cntlline_t = cntlline;
				   }
			   }
			/* Initialize variable */
			for( i=0; i<256; i++ ) cntchar_ascii[i] = 0;
				cntchar = cntword = cntline = 0;

			listFiles++;
		    }
	   }
        else
	   {
		errors = cnt( flname );
		if( errors )
			show_errors( errors );
		else
			show_cnt( "", cntchar, cntword, cntline, cntchar_ascii );
	   }

	if( !errors )
		if( total > 1 )
			show_cnt( "total",
				   cntchar_t, cntword_t, cntline_t, cntchar_ascii_t );

	/* Exit form programm with ERROR status
	*  if no error 0 return
	*  else number of errors
	*/
	return errors;
}

/*----------------------------------------------
  End of wcn.c
-----------------------------------------------*/
