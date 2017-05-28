/*
 * Module name: VBT parser and re-shuffler
 *
 *** WARNING: this VBT parser and re-shuffler is NOT finished YET! ***
 *
 * Author: _nobody_ (nobodyless@gmail.com)
 * Copyright _nobody_ 2017, GPL3 Licence
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define	LF	10
#define	CR	13

struct vbt_hdr {
	char		vbt_signature[20];
	uint16_t	vbt_version;
	uint16_t	vbt_header_size;
	uint16_t	vbt_size;
	uint16_t	vbt_checksum;
	uint32_t	bdb_offset;
};

struct bdb_hdr {
	char		bdb_signature[16];
	uint16_t	bdb_version;
	uint16_t	bdb_header_size;
	uint16_t	bdb_size;
};

void print_vbt_header_info(struct vbt_hdr *vh) {
  //	printf("VBT Signature is: %s\n", vh->vbt_signature);
	printf("VBT Version Info is: %8x [%d]\n", vh->vbt_version, vh->vbt_version);
	printf("VBT Header Size is: %8x [%d]\n", vh->vbt_header_size, vh->vbt_header_size);
	printf("VBT Size is: %8x [%d]\n", vh->vbt_size, vh->vbt_size);
	printf("VBT Checksum is: %x\n", vh->vbt_checksum);
	printf("BDB Offset is: %x\n", vh->bdb_offset);
}

void print_bdb_header_info(struct bdb_hdr *bh) {
  //	printf("BDB Signature is: %s\n", bh->bdb_signature);
	printf("BDB Version Info is: %8x [%d]\n", bh->bdb_version, bh->bdb_version);
	printf("BDB Header Size is: %8x [%d]\n", bh->bdb_header_size, bh->bdb_header_size);
	printf("BDB Size is: %8x [%d]\n", bh->bdb_size, bh->bdb_size);
}

int main(int argc, char *argv[]) {
	FILE		*infile, *outfile;
	struct		stat st;
	char		*in_record, *in_default;
	uint32_t	i;
	uint32_t	*size32_ptr;
	size_t		ifile_size, read_size;
	uint16_t	*size16_ptr;
	struct vbt_hdr	vbt_header;
	struct bdb_hdr	bdb_header;
	uint8_t		first_bdb_set[] = {0x01, 0x00, 0x00, 0x01};

	/*
	 * Usage: ./vbtp infile outfile
	 */
	if (3 != argc) {
		printf("Usage: ./vbtp infile outfile\n");
		return (-1);
	}

	stat(argv[1], &st);
	if (0 == (ifile_size = st.st_size)) {
		printf ("Zero size input file %s\n", argv[1]);
		return (-2);
	}
	else printf ("Size of input file is %d\n", ifile_size);

	// open infile
	infile = fopen(argv[1], "r");
	if (NULL == infile) {
		printf("Error opening input file - not found.\n");
		return (-3);
	}

	// open outfile
	outfile = fopen(argv[2], "w");
	if (NULL == outfile) {
		printf("Error opening output file - name missing.\n");
		return (-4);
	}

	// Allocate infile size characters to the in_record
	in_default = in_record = malloc(ifile_size);
	if (!in_default) {
		printf("Out of memory, could NOT allocate\n");
		return (-5);
	}
	// bzero((char *)&in_default, ifile_size);

	/* Read the entire file into memory. */
	if(0 == (read_size = fread(in_record, sizeof(char), ifile_size, infile))) return (-6);
	if (read_size != ifile_size) return (-77);
	//	if (NULL == (in_record = fgets(in_record, ifile_size, infile))) return (-6);

	/*
	 *  [Task 1] - Find and parse VBT Header (defined as struct vbt_hdr)!
	 */
	// Scour memory looking for the VBT signature
	for (i = 0; i + 4 < ifile_size; i++) {
		if (!memcmp(in_record, "$VBT", 4)) break;
		in_record++;
	}

	if (ifile_size - 4 == i) {
		printf("string $VBT not found, thus VBT Header were not found!\n");
		return (-7);
	}
	printf("string $VBT found, on the %d position\n", i);
	// strncpy(&vbt_header.vbt_signature, &in_default[i], 20);
	//	vbt_header.vbt_signature[21]=0;
	// Move buffer pointer beyound string "$VBT" + 16
	in_record += 20;

	// Video BIOS Version Info (vbt_header.vbt_version)
	size16_ptr = (uint16_t *)in_record;
	vbt_header.vbt_version = *size16_ptr;
	in_record++;
	in_record++;

	// Find VBT Header Size
	size16_ptr = (uint16_t *)in_record;
	vbt_header.vbt_header_size = *size16_ptr;
	in_record++;
	in_record++;

	// Find VBT Size
	size16_ptr = (uint16_t *)in_record;
	vbt_header.vbt_size = *size16_ptr;
	in_record++;
	in_record++;

	// Find VBT Checksum
	size16_ptr = (uint16_t *)in_record;
	vbt_header.vbt_checksum = *size16_ptr;
	in_record++;
	in_record++;

	// Find BDB Offset
	size32_ptr = (uint32_t *)in_record;
	vbt_header.bdb_offset = *size32_ptr;
	in_record += 4;

	// VBT Header details
	print_vbt_header_info(&vbt_header);

	// Write to outfile VBT Header content
	fwrite(&in_default[i], sizeof(vbt_header), sizeof(char), outfile);

	// Find BDB Header
	i = vbt_header.bdb_offset;
	in_record = in_default + i;

	/*
	 *  [Task 2] - Find and parse BDB Header (defined as struct vbt_hdr)!
	 */
	// Scour memory looking for the BDB signature
	for (i; i + 16 < ifile_size; i++) {
		if (!memcmp(in_record, "BIOS_DATA_BLOCK ", 16)) break;
		in_record++;
	}

	if (ifile_size - 16 == i) {
		printf("string BIOS_DATA_BLOCK not found, thus BDB Header were not found!\n");
		return (-8);
	}

	printf("string BIOS_DATA_BLOCK found, on the %d position\n", i);
	// strncpy(&vbt_header.vbt_signature, &in_default[i], 20);
	//	vbt_header.vbt_signature[21]=0;
	// Move buffer pointer beyound string "BIOS_DATA_BLOCK " + 16
	in_record += 16;

	// BDB Version Info (vbt_header.vbt_version)
	size16_ptr = (uint16_t *)in_record;
	bdb_header.bdb_version = *size16_ptr;
	in_record++;
	in_record++;

	// Find BDB Header Size
	size16_ptr = (uint16_t *)in_record;
	bdb_header.bdb_header_size = *size16_ptr;
	in_record++;
	in_record++;

	// Find BDB Size
	size16_ptr = (uint16_t *)in_record;
	bdb_header.bdb_size = *size16_ptr;
	in_record++;
	in_record++;

	//BDB Header details
	print_bdb_header_info(&bdb_header);

	// Write to outfile BDB Header content
	fwrite(&in_default[i], sizeof(bdb_header), sizeof(char), outfile);

	// Move beyound BDB Header
	i = vbt_header.bdb_offset + bdb_header.bdb_header_size;
	in_record = in_default + i;

	/*
	 *  [Task 3] - Find and parse all BDB Blocks!
	 */
	// FAKE CALCULATION (To BE Determined) look for the first BDB block
	for (i; i + 4 < ifile_size; i++) {
		if (!memcmp(in_record, (char *)first_bdb_set, 4)) break;
		in_record++;
	}

	for (i=0; i<8; i++) {
		printf(" %x", (uint8_t)*in_record);
		in_record++;
	}
	printf("\n");

	free (in_default);
	fclose(infile);
	fclose(outfile);
	return (0);
}
