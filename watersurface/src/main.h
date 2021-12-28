
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

#define IMGH	320	// image forest height
#define WATERH	(HEIGHT-IMGH) // water height

// format: 4-bit pixel graphics
// image width: 640 pixels
// image height: 320 lines
// image pitch: 320 bytes
extern const u8 ForestImg[102400] __attribute__ ((aligned(4)));

// format: 4-bit pixel graphics
// image width: 512 pixels
// image height: 58 lines
// image pitch: 256 bytes
extern const u8 DuckImg[14848] __attribute__ ((aligned(4)));

// sound format: PCM mono 8-bit 22050Hz
extern const u8 RiverSnd[105000];

#endif // _MAIN_H
