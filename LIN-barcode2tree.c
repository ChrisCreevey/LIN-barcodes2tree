#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TEMP
#define TEMP 2
#endif

#define NUMBARCODES 1000
#define BARCODELEN 14
#define STRINGLEN 1000000
#define	BIGSTRING 1000000

int num_remaining_barcodes =0;

void cluster_barcode(char **barcode_array, char **seq_names);

int main(int argc, char *argv[]){
	int i, k;
	char **seq_names = NULL, **barcode_array = NULL, string[STRINGLEN], c , *token, outfilename[1000];
	FILE *inputfile = NULL, *outputfile = NULL;

	string[0] = '\0';
	/*create matrices holding data */

	barcode_array = malloc(NUMBARCODES*sizeof(char*));
	seq_names = malloc(NUMBARCODES*sizeof(char*));

	for(i=0; i<NUMBARCODES; i++){
		barcode_array[i]=malloc(STRINGLEN*sizeof(char));
		seq_names[i]=malloc(STRINGLEN*sizeof(char));
		barcode_array[i][0] = seq_names[i][0] = '\0';
	}

	if((inputfile = fopen(argv[1], "r")) == NULL)   /* check to see if the file is there */
	    {                          /* Open the fundamental tree file */
	    fprintf(stderr, "Error: Cannot open list file %s\n", argv[1]);
	    exit(1);
	    }

	/* read in the barcode data */
	/*inputfile = fopen("input_barcodes.txt", "r"); */
	i=0;
	while(!feof(inputfile)){
		k=0;
		/*read in entire line */
		while(!feof(inputfile) && (c=getc(inputfile)) != '\n' && c != '\r') {
			string[k]=c; 
			k++;
		}
		string[k] = '\0';
		if(!feof(inputfile)){
			/*printf("%s\n", string); */
			/*split string but tab character */
			token = strtok(string, "\t"); /* get first part of string (the name) */
			strcpy(seq_names[i], token);
			token = strtok(NULL, "\t"); /* get second part of string (the barcode) */
			strcpy(barcode_array[i], token);

			/*if(!feof(inputfile)) printf("%s\t%s\n",seq_names[i], barcode_array[i]); */
			if(i<NUMBARCODES) i++;
		}
	}
	num_remaining_barcodes = i;
	fclose(inputfile);

	/* cluser barcodes */
	cluster_barcode(barcode_array, seq_names);

	outfilename[0]='\0';
	strcpy(outfilename, argv[1]);
	strcat(outfilename, ".outtree.ph");
	outputfile = fopen(outfilename, "w");
	/* print out clustered barcodes */
	for(i=0; i<num_remaining_barcodes; i++) {
		if(strcmp(seq_names[i], "") != 0 ) fprintf(outputfile, "%s;\n",seq_names[i]);
	}
	fclose(outputfile);
}




void cluster_barcode(char **barcode_array, char **seq_names){
	/* The function recieves as input the barcodes and names and carries out the follow steps: */
	/* LOOP FOR THE LENGTH OF THE BARCODE */
	/** 	FOR EACH REMAINING BARCODE IN THE ARRAY **/
	/*** 		CHECK IF BARCODE STRING IS IDENTICAL TO ANY OTHER IN THE MATRIX ***/
	/*** 		GROUP ALL IDENTICAL BARCODES INTO A SINGLE REPRESENTATIVE WHERE THE NAME IS CONCATENATED IN THE FOLLOWING FORM: */
	/*** 			"(", BARCODE-NAME1, ",", BARCODE-NAME2, [ CONTINUE TO ADD ",", BARCODE-NAMEX FOR ALL SUBSEQUENT IDENTICAL BARCODES ], ")" ***/
	/*** 		REMOVE LAST NUMBER FROM ALL REMAINING BARCODES AND START INNER LOOP AGAIN ***/
	/* AT END OF OUTER LOOP CONCATENATE ALL REMAINING BARCODES IN MATRIX USING THE SAME APPROACH AS ABOVE, BUT WITH ADDITIONAL ";" AT END */
	/* RETURN FINAL STRING OF CLUSTERED BARCODES */
	int i=0, j=0, k=0, l=0, identical[NUMBARCODES], num_identical=0;
	char string[BIGSTRING], rep_barcode[STRINGLEN];

	string[0] ='\0';

	for(i=BARCODELEN-1; i>=0; i--) { /* LOOP FOR THE LENGTH OF THE BARCODE */

		for(j=0; j<num_remaining_barcodes; j++){ /* FOR EACH REMAINING BARCODE IN THE ARRAY **/
			for(k=0; k<num_remaining_barcodes; k++) identical[k] = 0; /* reset identical array */
			num_identical=0;
			/* identify and capute the array position of all barcodes that are the same as this one */
			for(k=j; k<num_remaining_barcodes; k++) { /* CHECK IF BARCODE STRING IS IDENTICAL TO ANY OTHER IN THE MATRIX */
				if(strcmp(barcode_array[k],"") != 0) { 
					if(strcmp(barcode_array[k], barcode_array[j]) == 0){
						identical[k]=1;
						num_identical++;
					}
				}
			}
			if(num_identical > 1){ /* GROUP ALL IDENTICAL BARCODES INTO A SINGLE REPRESENTATIVE */
				rep_barcode[0] = '\0';
				string[0] = '\0';
				strcpy(string, "(");
				for(k=0; k<num_remaining_barcodes; k++) {
					if(identical[k] == 1) {
						strcpy(rep_barcode, barcode_array[k]);
						strcat(string, seq_names[k]);
						strcat(string, ",");
						barcode_array[k][0] = '\0';
						seq_names[k][0] = '\0';
					}
				}
				string[strlen(string)-1] = ')';
				strcpy(barcode_array[num_remaining_barcodes], rep_barcode);
				strcpy(seq_names[num_remaining_barcodes], string);
				num_remaining_barcodes++;
			}

		}
		/*** REMOVE LAST NUMBER FROM ALL REMAINING BARCODES ***/
		for(j=0; j<num_remaining_barcodes; j++){
			k=0; l=0;
			while(k!=i) {
				if(barcode_array[j][l] == ',') k++;
				l++;
			}
			barcode_array[j][l] = '\0';
		}

	}
	
}