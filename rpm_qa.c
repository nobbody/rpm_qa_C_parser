/*
 * Author: nobodyless@gmail.com, GPL2 copyright!
 *
 * Copyright _nobody_, Y2017. rpm -qa parser, which makes from
 * the command rpm -qa > input_file.txt simple bash shell script
 * in the case somebody needs to reinstall from schratch Fedora
 * distro. Also could be used for other distros, after original
 * C code re-edit!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LF 10
#define CR 13

int main(int argc, char *argv[]) {
	FILE		*infile, *outfile;
	char		*in_record, *in_default;
	const char	dnf[] = {"dnf install "};
	int		i, adjustment = 0;

	/*
	 * Usage: ./rpm_qa infile outfile [dnf] (dnf string optional)
	 */
	if (3 > argc && 4 < argc) {
		printf("Usage: ./rpm_qa infile outfile [dnf] (dnf string optional\n");
		return (1);
	}

	// open infile
	infile = fopen(argv[1], "r");
	if (NULL == infile) {
		printf("Error opening input file - not found.\n");
		return (2);
	}

	// open outfile
	outfile = fopen(argv[2], "w");
	if (NULL == outfile) {
		printf("Error opening output file - name missing.\n");
		return (3);
	}

	// Allocate 80 characters to the in_record
	in_default = in_record = malloc(80 * sizeof(char));
	if (4 == argc) {
		adjustment = sizeof(dnf) - 1;
		if (0 == strcmp(argv[3], "dnf")) in_record = strcpy(in_record, &dnf[0]);
	}
	in_record += adjustment;
	// bzero((char *)&in_default, 80);

	while (EOF != (*in_record = (char)fgetc(infile))) {
		i = 0;
		while (LF != *in_record) {
			// Test printing, intoduced here for debugging purposes
			// printf ("%c", *in_record);
			if (isdigit(*in_record)) {
				in_record--;
				if ('-' == *in_record) break;
				in_record++;
			}
			in_record++;
			*in_record = (char)fgetc(infile);
			i++;
		}
		*in_record = LF;
		while (LF != (char)fgetc(infile));
		// Test printings, intoduced here for debugging purposes
		// printf ("%c", *in_record);
		// printf ("\ncurrent line is: %s\n", &in_default[0]);
		fwrite (&in_default[0], i + adjustment, sizeof(char), outfile);
		in_record = in_default + adjustment;
	}

	free (in_default);
	fclose(infile);
	fclose(outfile);
	return (0);
}
