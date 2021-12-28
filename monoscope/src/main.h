
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

// format: 4-bit pixel graphics
// image width: 640 pixels
// image height: 480 lines
// image pitch: 320 bytes
#if COLOR_FORMAT == COLOR_FORMAT_RGB // color format RGB: bit2 red, bit1 green, bit0 blue (recommended)
extern const u8 MonoscopeRGBImg[153600] __attribute__ ((aligned(4)));
#elif COLOR_FORMAT == COLOR_FORMAT_BGR // color format BGR: bit2 blue, bit1 green, bit0 red
extern const u8 MonoscopeBGRImg[153600] __attribute__ ((aligned(4)));
#else // COLOR_FORMAT == COLOR_FORMAT_GRB, color format GRB: bit2 green, bit1 red, bit0 blue
extern const u8 MonoscopeGRBImg[153600] __attribute__ ((aligned(4)));
#endif // COLOR_FORMAT

#endif // _MAIN_H
