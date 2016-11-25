#include "stdio.h"
#include "stdlib.h"


/***********************************************************/
/******************  Macro file Generator   ****************/
/***********************************************************/
/*                                                         */
/*  	This program takes the vFloors file that is created */
/*	after invoking HCompV and generates a Macro file   */
/*	required for further  HMM training                 */
/*                                                         */
/***********************************************************/
/*    Title :	macro_gen.c			  	   */
/*    Author:	Gary Lee                                   */
/*    Date:	30 March 1998                              */
/*    Version:	1                                          */
/*    Change History:                                      */
/*    Author of modifications:	G?nter Hirsch / Ericsson   */
/*      - 2 additional input parameters defined to achieve */
/*        a more global usability                          */
/*        - <VECSIZE>  and  <PARAMETER_TYPE>               */
/*      - output file name also input parameter            */
/*                                                         */
/***********************************************************/

/*===================================================================*/
/* Copyright(c) 1998 MOTOROLA LAND MOBILE PRODUCTS SECTOR            */
/* Jays Close, Viables Ind. Estate                                   */
/* Basingstoke, England                                              */
/*-------------------------------------------------------------------*/
/* ALL RIGHTS RESERVED:                                              */
/* No part of this software may be reproduced in any form            */
/* or by any means or used to make any derivative work               */
/* (such as transformation or adaptation) without the                */
/* authorisation of the relevant authority in MOTOROLA'S LAND        */
/* MOBILE PRODUCTS SECTOR.                                           */
/*===================================================================*/


int main(int argc, char *argv[])
{
	FILE *fp;
	FILE *outfile;
	char c;
	
	if(argc != 5)
	{	printf("Usage: %s VECSIZE PARAMETER_TYPE infile outfile\n", argv[0]);
		exit(1);
	}
	
	if ( (fp = fopen(argv[3], "r")) == NULL)
	{
		fprintf(stderr, "cannot open file %s!\n", argv[3]);
		exit(-1);
	}
	if ( (outfile = fopen(argv[4], "w")) == NULL)
	{
		fprintf(stderr, "cannot open file %s!\n", argv[4]);
		exit(-1);
	}
	
	fprintf(outfile, "~o\n<STREAMINFO> 1 %s\n", argv[1]);
	fprintf(outfile, "<VECSIZE> %s\n", argv[1]);
	fprintf(outfile, "<NULLD>\n<%s>\n", argv[2]);	
	while((c=getc(fp))!=EOF)
		fprintf(outfile,"%c",c);
	
	fclose(fp);
	fclose(outfile);
	return(0);
}
