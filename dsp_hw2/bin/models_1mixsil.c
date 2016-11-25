#include "stdio.h"
#include "string.h"

#define FALSE 0
#define TRUE 1

void silmodel(FILE *, FILE *);

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
/*    Change History: version 1.1     7th July 1998        */
/*                    cure of seg fault on sun             */
/*              char c, entry[9];  ->  char c, entry[90];  */
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
	int  CAT=FALSE, i;
	FILE *fp;
	FILE *outfile;
	
	char c, entry[90];
		
	outfile = fopen(argv[2], "w");
	
	for (i=1; i<12; i++) 
	{    
		 fp = fopen(argv[1], "r");
	 	CAT=FALSE;
	 	while(CAT==FALSE)  
		{
	  		fscanf(fp,"%s",entry);
	  		if(strncmp(entry,"\"hmmdef\"",8)==0) 
	  		
	  		{	
				if(i==1) fprintf(outfile,"~h \"liN\"");
				if(i==2) fprintf(outfile,"~h \"#i\"");
				if(i==3) fprintf(outfile,"~h \"#er\"");
				if(i==4) fprintf(outfile,"~h \"san\"");
				if(i==5) fprintf(outfile,"~h \"sy\"");
				if(i==6) fprintf(outfile,"~h \"#u\"");
				if(i==7) fprintf(outfile,"~h \"liou\"");
				if(i==8) fprintf(outfile,"~h \"qi\"");
				if(i==9) fprintf(outfile,"~h \"ba\"");
				if(i==10) fprintf(outfile,"~h \"jiou\"");
				if(i==11) { silmodel(fp,outfile);
					     break; }
				CAT=TRUE;
			}
		}
		if(CAT==TRUE)
		{
	        while((c=getc(fp))!=EOF) fprintf(outfile,"%c",c);		
		fclose(fp);
		
		}
	}
	return(0);
}

/***********************************************************/
/******************  Sil Model Generator    ****************/
/***********************************************************/
/*                                                         */
/*            this function creates a 5 states             */
/*              10 Mixture silence model                   */
/*                                                         */
/*                                                         */
/***********************************************************/
/*    Author:	Gary Lee                                   */
/*    Date:		30 March 1998                      */
/*    Version:	1                                          */
/*    Change History:                                      */
/*                                                         */
/***********************************************************/
/***********************************************************/

void silmodel(FILE *fp, FILE *outfile)
{
	int j=0, k,CAT=FALSE, line_no=0, STOP=FALSE; 
	char line[1500],buf[7][1500]; /*** ATTENTION: changed from 300 -> 1500***/
	
	printf("CREATING SILENCE MODEL\n");
	fprintf(outfile,"~h \"sil\"\n");
	fprintf(outfile,"<BEGINHMM>\n<NUMSTATES> 5\n");
	
	while(STOP!=TRUE)
	{
		fgets(line, 1500, fp); /*** ATTENTION: changed from 300 -> 1500***/
		if(strncmp(line,"<MEAN>",6)==0) 
		{
			if(j==0) CAT=TRUE;
			else 
			{
				CAT=FALSE;
				STOP=TRUE;
			}
			j++;
		}
		
		if(CAT==TRUE)
			  	
			{
				
				strcpy(buf[line_no++],line); 
/*				printf("line_no %d ::  %s\n",line_no-1, line);*/
			}
		
	}
	
	for(k=2; k<5; k++)
	{
	
		fprintf(outfile,"<STATE> %d\n<NUMMIXES> 1\n",k);
		
			
			for(j=0; j<line_no-1; j++) fputs(buf[j],outfile);
			/*for(j=0; j<line_no-1; j++) printf("line %d :: %s
			\n",j,buf[j]);*/
		
	}
	fprintf(outfile,"<TRANSP> 5\n0.000000e+00 1.000000e+00 0.000000e+00 0.000000e+00 0.000000e+00\n");
	fprintf(outfile,"0.000000e+00 6.000000e-01 4.000000e-01 0.000000e+00 0.000000e+00\n");
 	fprintf(outfile,"0.000000e+00 0.000000e+00 6.000000e-01 4.000000e-01 0.000000e+00\n");
 	fprintf(outfile,"0.000000e+00 0.000000e+00 0.000000e+00 7.000000e-01 3.000000e-01\n");
 	fprintf(outfile,"0.000000e+00 0.000000e+00 0.000000e+00 0.000000e+00 0.000000e+00\n<ENDHMM>\n");

}	
	
	
