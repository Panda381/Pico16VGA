
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

int main()
{
	float phase;
	int y, y0, y2, duckx;

	// play sound
	PlaySound(RiverSnd, sizeof(RiverSnd), True);

	// display top part of the screen
	memcpy(FrameBuf, ForestImg, sizeof(ForestImg));

	// main loop
	phase = 0;
	duckx = 650;
	while (true)
	{
		// to avoid duck flickering
		WaitVSync();

		// animate water
		for (y = IMGH; y < HEIGHT; y++)
		{

#define WAVEAMP		0.15f	// wave amplitude
#define WAVEPERSP	30.0f	// perspective deformation
#define WAVELEN		200	// wave length
#define ANIMSPEED	0.4f	// animation speed

			y0 = y - IMGH; // Y relative to top of water
			y2 = (int)(IMGH - 1 - (sinf(phase + WAVELEN/sqrtf(y0/WAVEPERSP + 1))*WAVEAMP + 1)*y0);
			if (y2 < 0) y2 = 0;
			if (y2 >= IMGH) y2 = IMGH-1;
			memcpy(&FrameBuf[y*WIDTHBYTE], &ForestImg[y2*WIDTHBYTE], WIDTHBYTE);
		}

		// display duck
		DrawBlit(DuckImg+(duckx & 3)*64, duckx, 330, 0, 0, 128, 58, 512, 58, COL_MAGENTA);		

		// increase animation phase
		phase += ANIMSPEED;

		// shift duck
		duckx--;
		if (duckx < -250) duckx = 650;

		// delay
		sleep_ms(50);
	}
}
