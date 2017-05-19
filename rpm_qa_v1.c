/*
 * Author: nobodyless@gmail.com, GPL3 copyright!
 *
 * Another, simplified version of original parser, ment to be
 * faster and saver:
 * [1] malloc() is swapped with the stack buffer variable;
 * [2] Instead fgetc(), fgets() used, to fetch whole record at once!
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
	char		buffer[100];
	const char	dnf[] = {"dnf install "};
	int		i, adjustment = 0;

	/*
	 * Usage: ./rpm_qa_v1 infile outfile [dnf] (dnf string optional)
	 */
	if (3 > argc && 4 < argc) {
		printf("Usage: ./rpm_qa infile outfile [dnf] (dnf string optional\n");
		return (1);
	}

	// Open infile
	infile = fopen(argv[1], "r");
	if (NULL == infile) {
		printf("Error opening input file - not found.\n");
		return (2);
	}

	// Open outfile
	outfile = fopen(argv[2], "w");
	if (NULL == outfile) {
		printf("Error opening output file - name missing.\n");
		return (3);
	}

	// Allocate local stack variable buffer[100] to the in_record
	in_default = in_record = &buffer[0];
	if (4 == argc) {
		adjustment = sizeof(dnf) - 1;
		if (0 == strcmp(argv[3], "dnf")) in_record = strcpy(in_record, &dnf[0]);
	}
	in_record += adjustment;

	while (NULL != (in_record = fgets(in_record, sizeof(buffer) - adjustment, infile))) {
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
			i++;
		}
		*in_record = LF;
		// Test printings, intoduced here for debugging purposes
		// printf ("%c", *in_record);
		// printf ("\ncurrent line is: %s\n", &in_default[0]);
		fwrite (&in_default[0], i + adjustment, sizeof(char), outfile);
		in_record = in_default + adjustment;
	}

	fclose(infile);
	fclose(outfile);
	return (0);
}
