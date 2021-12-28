// Palettes for Pico16VGA
// - Prepared for parallel digital interface with YRGB colors.

#include <stdio.h>

#define FILENAME1RGB "pal16_RGB.act"
#define FILENAME2RGB "pal16_RGB.txt"
#define FILENAME1BGR "pal16_BGR.act"
#define FILENAME2BGR "pal16_BGR.txt"
#define FILENAME1GRB "pal16_GRB.act"
#define FILENAME2GRB "pal16_GRB.txt"

#pragma warning(disable : 4996) // unsafe fopen

// palette BGR (recommended):
//   bit 0 = blue, bits B5 and B4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 1 = green, bits G5 and G4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 2 = red, bits R5 and R4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 3 = level, bits B3..B0, G3..G0, R3..R0, value 15 (from 63) = 23.8% = 61 (from 255)

// palette RGB:
//   bit 0 = red, bits R5 and R4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 1 = green, bits G5 and G4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 2 = blue, bits B5 and B4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 3 = level, bits B3..B0, G3..G0, R3..R0, value 15 (from 63) = 23.8% = 61 (from 255)

// palette BRG:
//   bit 0 = blue, bits B5 and B4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 1 = red, bits R5 and R4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 2 = green, bits G5 and G4, value 48 (from 63) = 76.2% = 194 (from 255)
//   bit 3 = level, bits B3..B0, G3..G0, R3..R0, value 15 (from 63) = 23.8% = 61 (from 255)

int main(int argc, char* argv[])
{
	FILE* f1RGB = fopen(FILENAME1RGB, "wb");
	if (f1RGB == NULL)
	{
		printf("Error opening " FILENAME1RGB "\n");
		return 1;
	}

	FILE* f2RGB = fopen(FILENAME2RGB, "wb");
	if (f2RGB == NULL)
	{
		printf("Error opening " FILENAME2RGB "\n");
		return 1;
	}

	FILE* f1BGR = fopen(FILENAME1BGR, "wb");
	if (f1BGR == NULL)
	{
		printf("Error opening " FILENAME1BGR "\n");
		return 1;
	}

	FILE* f2BGR = fopen(FILENAME2BGR, "wb");
	if (f2BGR == NULL)
	{
		printf("Error opening " FILENAME2BGR "\n");
		return 1;
	}

	FILE* f1GRB = fopen(FILENAME1GRB, "wb");
	if (f1GRB == NULL)
	{
		printf("Error opening " FILENAME1GRB "\n");
		return 1;
	}

	FILE* f2GRB = fopen(FILENAME2GRB, "wb");
	if (f2GRB == NULL)
	{
		printf("Error opening " FILENAME2GRB "\n");
		return 1;
	}

	int i;
	unsigned char d[3];
	unsigned char b0, b1, b2, b3;
	for (i = 0; i < 16; i++)
	{
		b0 = (i & 1)*194;			// bit0 color component
		b1 = ((i >> 1) & 1) * 194;	// bit1 color component
		b2 = ((i >> 2) & 1) * 194;	// bit2 color component
		b3 = ((i >> 3) & 1) * 61;	// bit3 color component (intensity)

		b0 += b3;
		b1 += b3;
		b2 += b3;

		d[0] = b2;		// red = bit2
		d[1] = b1;		// green = bit1
		d[2] = b0;		// blue = bit0
		fwrite(d, 1, 3, f1RGB);
		fprintf(f2RGB, "%d: %u,%u,%u (0x%02X%02X%02X)\n", i, d[0], d[1], d[2], d[0], d[1], d[2]);

		d[0] = b0;		// red = bit0
		d[1] = b1;		// green = bit1
		d[2] = b2;		// blue = bit2
		fwrite(d, 1, 3, f1BGR);
		fprintf(f2BGR, "%d: %u,%u,%u (0x%02X%02X%02X)\n", i, d[0], d[1], d[2], d[0], d[1], d[2]);

		d[0] = b1;		// red = bit1
		d[1] = b2;		// green = bit2
		d[2] = b0;		// blue = bit0
		fwrite(d, 1, 3, f1GRB);
		fprintf(f2GRB, "%d: %u,%u,%u (0x%02X%02X%02X)\n", i, d[0], d[1], d[2], d[0], d[1], d[2]);
	}

	d[0] = 0;
	d[1] = 0;
	d[2] = 0;
	for (; i < 256; i++)
	{
		fwrite(d, 1, 3, f1RGB);
		fwrite(d, 1, 3, f1BGR);
		fwrite(d, 1, 3, f1GRB);
	}

	fclose(f1RGB);
	fclose(f2RGB);
	fclose(f1BGR);
	fclose(f2BGR);
	fclose(f1GRB);
	fclose(f2GRB);

	return 0;
}
