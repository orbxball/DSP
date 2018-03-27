#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define FALSE 0
#define TRUE 1



/***********************************************************/
/****************** Model file Generator   ****************/
/***********************************************************/
/*                                                         */
/*            This program takes the hmmdef file and       */
/*              generates a HMM Model file                 */
/*                                                         */
/*                                                         */
/***********************************************************/
/*    Title : models_gen.c               		   */
/*    Author:	Gary Lee                                   */
/*    Date:		27 March 1998                      */
/*    Version:	1                                          */
/*    Change History:                                      */
/*                                                         */
/***********************************************************/
/* 							   */
/* 	The execution of htk's HCompV cmd generates a 	   */
/*	file (hmmdef) that gives the initial training      */
/* 	parameters; which is the same for each digit.      */
/*	However, as we model sil differently from the      */
/*	rest of the digits. We have to create a new	   */
/*	model file. model_gen.c creates a models file 	   */
/*	with sil explictly defined to a 5 states, 10	   */
/*	mixture model.					   */
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
	int  CAT=FALSE, STOP=FALSE, START=FALSE;
	FILE *fp;
	FILE *outfile;
	
	char line[300];

	if(argc !=3)
	{	printf("Usage:  infile outfile\n");
		exit(1);
	}
	
	
	
	fp = fopen(argv[1], "r");	
	outfile = fopen(argv[2], "a+");

	fprintf(outfile,"~h \"sp\"\n");
	fprintf(outfile,"<BEGINHMM>\n<NUMSTATES> 3\n<STATE> 2\n");
 	
 	 		
	while(STOP==FALSE)
	{
		fgets(line, 300, fp); 
		if(strncmp(line,"~h \"sil\"",8)==0) START=TRUE;
		if(START==TRUE) 
		{	if(strncmp(line,"<STATE> 3",9)==0) 
			{	CAT=TRUE;
				continue;
			}
			if(strncmp(line,"<STATE> 4",9)==0) 
			{	CAT=FALSE;
				STOP=TRUE;
				break;
			}
			if(CAT==TRUE)  	fprintf(outfile,"%s",line);
			
		}
	}


	fprintf(outfile,"<TRANSP> 3\n0.000000e+00 1.000000e+00 0.000000e+00\n");
	fprintf(outfile,"0.000000e+00 5.000000e-01 5.000000e-01\n");
 	fprintf(outfile,"0.000000e+00 0.000000e+00 0.000000e+00\n<ENDHMM>\n");
	fclose(fp);
	fclose(outfile);
	return(0);
}
