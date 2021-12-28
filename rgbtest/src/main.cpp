
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

int main()
{
	int y;

	// draw rainbow gradient
	for (y = 0; y < HEIGHT; y++) GenGrad(&FrameBuf[y*WIDTHBYTE], WIDTH);

	// draw text
	y = 220;
#define DX 38
	DrawRect(0, y, WIDTH, 32, COL_BLACK);
	y += 8;
	DrawText("Red", 30, y, 2, 2, COL_RED);
	DrawText("Yellow", 112, y, 2, 2, COL_YELLOW);
	DrawText("Green", 225, y, 2, 2, COL_GREEN);
	DrawText("Cyan", 340, y, 2, 2, COL_CYAN);
	DrawText("Blue", 449, y, 2, 2, COL_BLUE);
	DrawText("Magenta", 530, y, 2, 2, COL_MAGENTA);

	// gradient
#if COLOR_FORMAT == COLOR_FORMAT_RGB // color format RGB: bit2 red, bit1 green, bit0 blue (recommended)
	memcpy(&FrameBuf[252*WIDTHBYTE], GradientRGBImg, sizeof(GradientRGBImg));
#elif COLOR_FORMAT == COLOR_FORMAT_BGR // color format BGR: bit2 blue, bit1 green, bit0 red
	memcpy(&FrameBuf[252*WIDTHBYTE], GradientBGRImg, sizeof(GradientBGRImg));
#else // COLOR_FORMAT == COLOR_FORMAT_GRB, color format GRB: bit2 green, bit1 red, bit0 blue
	memcpy(&FrameBuf[252*WIDTHBYTE], GradientGRBImg, sizeof(GradientGRBImg));
#endif // COLOR_FORMAT

	while (true) {}
}
