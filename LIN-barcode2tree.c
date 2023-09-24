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

#define BARCODELEN 14
#define STRINGLEN 1000000
#define	BIGSTRING 1000000

int num_remaining_barcodes =0, NUMBARCODES = 0;

void cluster_barcode(char **barcode_array, char **seq_names);

int main(int argc, char *argv[]){
	int i, k;
	char **seq_names = NULL, **barcode_array = NULL, string[BIGSTRING], c , *token, outfilename[1000];
	FILE *inputfile = NULL, *outputfile = NULL;

   if(argc < 2)
        {
        printf("\nLIN-barcode2tree is designed to generate a phylogenetic tree based on the LIN coding provided. \
        	\n\nUsage: LIN-barcode2tree <INFILE>\n\tWhere <INFILE> contains the Names and LIN barcodes in a tab-delimited format as in the example file `input_barcodes.txt` provided.\
        	\n\tThe phylogenetic tree is outputted to a phylip-formatted tree filecalled <INFILE>.outtree.ph \
        	\n\tPLEASE-NOTE: Ensure there are no \",\", \"(\" or \")\" characters in the names.\n\n");
        exit(1);
        }
    



	string[0] = '\0';
	/*create matrices holding data */

	/* open the file containing the barcodes */
	if((inputfile = fopen(argv[1], "r")) == NULL)   /* check to see if the file is there */
	    {                          /* Open the input  file */
	    fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
	    exit(1);
	    }

	/* do an initial scan through the file to count how many barcodes are in the file */
	while(!feof(inputfile)) {
		if((c = getc(inputfile)) == '\n' || c == '\r') NUMBARCODES++;
	}
	NUMBARCODES = NUMBARCODES*2; /* we need twice the number of barcodes as there will be up to n-2 internal clusters if fully bifurcated */
	rewind(inputfile);  /* reset file reader to beginning */

	barcode_array = malloc(NUMBARCODES*sizeof(char*));
	seq_names = malloc(NUMBARCODES*sizeof(char*));

	for(i=0; i<NUMBARCODES; i++){
		barcode_array[i]=malloc(STRINGLEN*sizeof(char));
		seq_names[i]=malloc(STRINGLEN*sizeof(char));
		barcode_array[i][0] = seq_names[i][0] = '\0';
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
			else printf("Warning: The number of barcodes is greater than the number of lines in the file \n\t Is there a problem iwith the input format?");
		}
	}
	num_remaining_barcodes = i;
	fclose(inputfile);

	/* cluser barcodes */
	cluster_barcode(barcode_array, seq_names);

	
	/* There may be several remaining clusters that need to be finally clustered together */
	string[0] = '\0'; i=0;
	for(k=0; k<num_remaining_barcodes; k++) {
		if(strcmp(seq_names[k], "") != 0) { /* if there is something in this array */
			i++;
			strcat(string, seq_names[k]);
			strcat(string, ",");
			barcode_array[k][0] = '\0';
			seq_names[k][0] = '\0';
		}
	}
	

	outfilename[0]='\0';
	strcpy(outfilename, argv[1]);
	strcat(outfilename, ".outtree.ph");
	outputfile = fopen(outfilename, "w");

	/* print out clustered barcodes */
	if(i>1) {
		string[strlen(string)-1] = ')';
		fprintf(outputfile, "(%s;\n",string);
	}
	else {
		string[strlen(string)-1] = ';';
		fprintf(outputfile, "%s\n",string);
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
	int i=0, j=0, k=0, l=0, *identical= NULL, num_identical=0;
	char string[BIGSTRING], rep_barcode[STRINGLEN];

	string[0] ='\0';
	identical = malloc(NUMBARCODES*sizeof(int));

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