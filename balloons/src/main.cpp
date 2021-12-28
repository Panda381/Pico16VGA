
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

#define BACKH	(HEIGHT/BACK_NUM)	// height of back buffer (=120)

// display back buffer
ALIGNED u8 BackBuf[WIDTHBYTE*BACKH];	// back buffer (320 * 120 = 38400 bytes)

// hot-air balloons
float hotx[HOTAIR_NUM];
float hoty[HOTAIR_NUM];
float hotdx[HOTAIR_NUM];
float hotdy[HOTAIR_NUM];

// party balloons
const u8* balimg_src[4] = { BlueImg, GreenImg, RedImg, YellowImg };
const u8* balimg[BALLOON_NUM];
int balx[BALLOON_NUM];
float baly[BALLOON_NUM];
float baldy[BALLOON_NUM];

// main function
int main()
{
	int i, x, y, back;
	float xf, yf;

	// current sky animation
	float skyx = 0;
	float skyy = 0;

	// hot-air balloons coordinates
	for (i = 0; i < HOTAIR_NUM; i++)
	{
		hotx[i] = RandFloatMax(WIDTH-HOTAIRW);
		hoty[i] = RandFloatMax(HEIGHT-HOTAIRH);
		hotdx[i] = RandFloatMinMax(HOTAIR_MINSPEED, HOTAIR_MAXSPEED)/BACK_NUM;
		if (RandU8() < 0x80) hotdx[i] = -hotdx[i];
		hotdy[i] = RandFloatMinMax(HOTAIR_MINSPEED, HOTAIR_MAXSPEED)/BACK_NUM;
		if (RandU8() < 0x80) hotdy[i] = -hotdy[i];
	}

	// prepare party balloons
	for (i = 0; i < BALLOON_NUM; i++)
	{
		balimg[i] = balimg_src[RandU8Max(3)];
		balx[i] = RandU16Max(WIDTH-BALLOONW);
		baly[i] = RandFloatMinMax(-BALLOONH, HEIGHT-1);
		baldy[i] = RandFloatMinMax(BALLOON_MINSPEED, BALLOON_MAXSPEED)/BACK_NUM;
	}

	// prepare back buffer
	back = 0;
	pDrawBuf = BackBuf;
	DrawBufY = 0;
	DrawBufH = BACKH;

	// main loop
	while (true)
	{
		// animate sky
		DrawImg(CloudsImg, (int)(skyx - CLOUDSW), (int)(skyy - CLOUDSH),	0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);
		DrawImg(CloudsImg, (int)(skyx), (int)(skyy - CLOUDSH),			0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);
		DrawImg(CloudsImg, (int)(skyx + CLOUDSW), (int)(skyy - CLOUDSH),	0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);

		DrawImg(CloudsImg, (int)(skyx - CLOUDSW), (int)(skyy),			0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);
		DrawImg(CloudsImg, (int)(skyx), (int)(skyy),				0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);
		DrawImg(CloudsImg, (int)(skyx + CLOUDSW), (int)(skyy),			0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);

		DrawImg(CloudsImg, (int)(skyx - CLOUDSW), (int)(skyy + CLOUDSH),	0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);
		DrawImg(CloudsImg, (int)(skyx), (int)(skyy + CLOUDSH),			0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);
		DrawImg(CloudsImg, (int)(skyx + CLOUDSW), (int)(skyy + CLOUDSH),	0, 0, CLOUDSW, CLOUDSH, CLOUDSW, CLOUDSH);

		skyx += CLOUDS_SPEEDX/BACK_NUM;
		if (skyx >= CLOUDSW) skyx -= CLOUDSW;
		skyy += CLOUDS_SPEEDY/BACK_NUM;
		if (skyy >= CLOUDSH) skyy -= CLOUDSH;

		// animate hot-air balloons
		for (i = 0; i < HOTAIR_NUM; i++)
		{
			xf = hotx[i] + hotdx[i];
			if (xf < 0)
			{
				xf = 0;
				hotdx[i] = RandFloatMinMax(HOTAIR_MINSPEED/BACK_NUM, HOTAIR_MAXSPEED/BACK_NUM);
			}

			if (xf > WIDTH - HOTAIRW)
			{			
				xf = WIDTH - HOTAIRW;
				hotdx[i] = -RandFloatMinMax(HOTAIR_MINSPEED/BACK_NUM, HOTAIR_MAXSPEED/BACK_NUM);
			}
			hotx[i] = xf;

			yf = hoty[i] + hotdy[i];
			if (yf < 0)
			{
				yf = 0;
				hotdy[i] = RandFloatMinMax(HOTAIR_MINSPEED/BACK_NUM, HOTAIR_MAXSPEED/BACK_NUM);
			}

			if (yf > HEIGHT - HOTAIRH)
			{			
				yf = HEIGHT - HOTAIRH;
				hotdy[i] = -RandFloatMinMax(HOTAIR_MINSPEED/BACK_NUM, HOTAIR_MAXSPEED/BACK_NUM);
			}
			hoty[i] = yf;

			DrawBlit(HotairImg, (int)hotx[i], (int)hoty[i], 0, 0, HOTAIRW, HOTAIRH, HOTAIRW, HOTAIRH, COL_WHITE);
		}

		// animate party balloons
		for (i = 0; i < BALLOON_NUM; i++)
		{
			yf = baly[i] - baldy[i];
			if (yf < -BALLOONH) yf += HEIGHT+BALLOONH;
			baly[i] = yf;

			DrawBlit(balimg[i], balx[i], (int)baly[i], 0, 0, BALLOONW, BALLOONH, BALLOONW, HOTAIRH, COL_WHITE);
		}

		// delay
//		sleep_ms(10);

		// update back buffer
		DrawBufUpdate();

		// shift back buffer
		back++;
		DrawBufY += BACKH;
		if (back == BACK_NUM)
		{
			back = 0;
			DrawBufY = 0;
		}
	}
}
