
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

int main()
{
	// copy image to the screen
#if COLOR_FORMAT == COLOR_FORMAT_RGB // color format RGB: bit2 red, bit1 green, bit0 blue (recommended)
	memcpy(FrameBuf, MonoscopeRGBImg, sizeof(MonoscopeRGBImg));
#elif COLOR_FORMAT == COLOR_FORMAT_BGR // color format BGR: bit2 blue, bit1 green, bit0 red
	memcpy(FrameBuf, MonoscopeBGRImg, sizeof(MonoscopeBGRImg));
#else // COLOR_FORMAT == COLOR_FORMAT_GRB, color format GRB: bit2 green, bit1 red, bit0 blue
	memcpy(FrameBuf, MonoscopeGRBImg, sizeof(MonoscopeGRBImg));
#endif // COLOR_FORMAT

	while (true) {}
}
