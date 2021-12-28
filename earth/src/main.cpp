
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

#define USERANDSHADOW	1	// 1=use random shadow (or rastered shadow otherwise)

// precalculated U coordinate
#define UNUM 1024
#define UNUM2 (UNUM/2)
int UTab[UNUM];

// main function
int main()
{
	int i, x, y, xr, u, v, off, xx, yy, xx2, yy2;
	u8* d;
	u8 c;
	const u8* s1;
	const u8* s2;
	const u8* s;

	// clear draw box
	DrawClear();

	// prepare U table
	for (i = 0; i < UNUM; i++)
	{
		UTab[i] = (int)((asinf((float)(i-UNUM2)/UNUM2)/PI2+0.25f)*EARTHW);
	}

	// main loop
	off = 0;
	while (true)
	{

#if USERANDSHADOW // use random shadow

		// re-initialize random number generator to start with the same seed
		// Note: maybe flickering shadow looks better
//		RandSetSeed(12345);

#endif // use random shadow

		// Y loop
		for (y = -GLOBER; y < +GLOBER; y++)
		{
			// X offset
			xr = (int)(sqrtf((float)GLOBER*GLOBER - y*y) + 0.5f);
			if (xr == 0) xr = 1; // to avoid divide by zero

			// destination address
			xx = GLOBEX-xr;
			yy = (y+GLOBEY);

			// texture V coordinate
			v = (int)((asinf((float)y/GLOBER)/PI+0.5f)*EARTHH);

			// source address
			s1 = Earth1Img;
			s2 = Earth2Img;

			// X loop
			for (x = -xr; x < xr; x++)
			{
				// texture U coordinate
				u = UTab[x*UNUM2/xr+UNUM2];

#define SHADOWBEG	(xr*7/16) // shadow beging
#define SHADOW1		(xr*8/16) // shadow boundary 1
#define SHADOW2		(xr*9/16) // shadow boundary 2
#define SHADOWEND	(xr*10/16) // shadow end

#if USERANDSHADOW // use random shadow

				// select bitmap
				if (x < SHADOWBEG) // always day
					s = s1;	// day
				else if (x >= SHADOWEND) // always night
					s = s2;	// night
				else
				{
					if (x <= (int)RandU16MinMax(SHADOWBEG, SHADOWEND))
						s = s1;	// day
					else
						s = s2;	// night
				}

#else // use random shadow

				if (x < SHADOWBEG) // always day
					s = s1;	// day
				else if (x >= SHADOWEND) // always night
					s = s2;	// night
				else if (x < SHADOW1) // light shadow
				{
					if (((x & 1) + ((y & 1)<<1)) == 0)
						s = s2;	// night
					else
						s = s1;	// day
				}
				else if (x < SHADOW2) // shadow
				{
					if (((x + y) & 1) == 0)
						s = s2;	// night
					else
						s = s1;	// day
				}
				else // dark shadow
				{
					if (((x & 1) + ((y & 1)<<1)) != 3)
						s = s2;	// night
					else
						s = s1;	// day
				}

#endif // use random shadow

				// draw pixel
				xx2 = (u + off + EARTHW/2) & (EARTHW-1);
				s = &s[v*EARTHW/2+xx2/2];
				c = ((xx2 & 1) == 0) ? (*s >> 4) : (*s & 0x0f);
				DrawPoint(xx, yy, c);
				xx++;
			}
		}

		// shift angle
		off -= 4;
	}
}
