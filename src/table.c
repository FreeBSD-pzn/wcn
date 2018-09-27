/*
 * Copyright (c) 2018 Oleg Pyzin. All rights reserved.
 *------------------
 * table.c
 *------------------
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


/* Description:
 * print value of characters array.
 * if max value of counters <   10000 then print 6 collumns
 * if max value of counters < 1000000 then print 5 collumns
 * in other - print 4 collumns
 * ----------------------------
 * Release is a very simple
 * using only standart and
 * system undependecies fucntions.
 * And a very simple algorithm.
 * -----------------------------
 * Using 80 collumns on the line
 *
 * --- Future ------------------
 * How many collumns may be on the screen?
 * get information from terminal settings
 * and decide how many collumns on the screen.
 *
 */

#include <stdio.h>
#include <ctype.h>

void show_table( unsigned int val[] )
{
	unsigned int max;
	int col, i;

	for( i=0, max=0; i<256 ;i++ )
		if( max < val[i] )
			max = val[i];

	if( max < 10000 )  /* 6 collumns */
		{
		for(i=0, col=0; i<256; i++)
			{
			if( val[i] )
				{
				if( isalnum(i) )
				  {
				   if( col<5 )
					{
					printf(" '%c'->%4d |", (unsigned char)i, val[i]);
					col++;
					}
				   else
					{
					printf(" '%c'->%4d\n", (unsigned char)i, val[i]);
					col = 0;
					}
				  }
				else
				  {
				   if( col<5 )
					{
                                        printf(" %3d->%4d |", i, val[i]);
					col++;
					}
				   else
					{
                                        printf(" %3d->%4d\n", i, val[i]);
					col = 0;
					}
				  }
				}    /* if( val[i] ) */
			}   /* for */
		}
	else if( max < 1000000 )   /* 5 collumns */
		{
		for(i=0, col=0; i<256; i++)
			{
                        if( val[i] )
                                {
                                if( isalnum(i) )
                                  {
                                   if( col<4 )
                                        {
                                        printf(" '%c'->%6d |", (unsigned char)i, val[i]);
                                        col++;
                                        }
                                   else
                                        {
                                        printf(" '%c'->%6d\n", (unsigned char)i, val[i]);
                                        col = 0;
                                        }
                                  }
                                else
                                  {
                                   if( col<4 )
                                        {
                                        printf(" %3d->%6d |", i, val[i]);
                                        col++;
                                        }
                                   else
                                        {
                                        printf(" %3d->%6d\n", i, val[i]);
                                        col = 0;
                                        }
                                  }
				}     /* if( val[i] ) */
			}   /* for */
		}
	else   /* 4 collumns */
		{
		for(i=0, col=0; i<256; i++)
			{
                        if( val[i] )
                                {
                                if( isalnum(i) )
                                  {
                                   if( col<3 )
                                        {
                                        printf(" '%c'->%10d |", (unsigned char)i, val[i]);
                                        col++;
                                        }
                                   else
                                        {
                                        printf(" '%c'->%10d\n", (unsigned char)i, val[i]);
                                        col = 0;
                                        }
                                  }
                                else
                                  {
                                   if( col<3 )
                                        {
                                        printf(" %3d->%10d |", i, val[i]);
                                        col++;
                                        }
                                   else
                                        {
                                        printf(" %3d->%10d\n", i, val[i]);
                                        col = 0;
                                        }
                                  }
				}   /* if( val[i] ) */
			}    /* for */
		}
	printf("\n");
}

/*----------------------------------------------
  End of table.c
-----------------------------------------------*/
