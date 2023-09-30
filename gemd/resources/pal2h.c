#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char **argv)
	{
	if (argc == 1)
		{
		fprintf(stderr, "Usage: pal2h <file.pal>\n");
		exit(-1);
		}
	
	FILE *fp = fopen(argv[1], "rb");
	if (fp == NULL)
		{
		fprintf(stderr, "Cannot open %s for read. Exiting.\n", argv[1]);
		exit(-2);
		}
	
	uint32_t magic;
	fread(&magic, 1, 4, fp);
	if (magic != '10AP')
		{
		fprintf(stderr, "%s is not a palette file. Exiting\n", argv[1]);
		exit(-3);
		}
	
	printf(
		"#ifndef _palette_header_\n"
		"#define _palette_header_\n"
		"/* Palette generated from %s */\n"
		"typedef struct\n"
		"	{\n"
		"	uint8_t r;\n"
		" 	uint8_t g;\n"
		"	uint8_t b;\n"
		"} RGB;\n\n"
		"typedef struct\n"
		"	{\n"
		" 	RGB rgb;\n"
		" 	uint8_t a;\n"
		"} RGBA;\n\n"
		"static RGB _palette[256] = {\n",
		argv[1]);
	
	for (int i=0; i<256; i++)
		{
		uint16_t rgb[3];
		fread(rgb, 6, 1, fp);
		for (int j=0; j<3; j++)
			rgb[j] = ntohs(rgb[j]);
		printf("\t/* %03d, 0x%02x */ {%d,%d,%d},\n", i, i,
			((int)rgb[0] * 255) / 1000,
			((int)rgb[1] * 255) / 1000,
			((int)rgb[2] * 255) / 1000
			);
		}
	printf("\t};\n"
		"#endif /* ! _palette_header_ */\n");
	
	}