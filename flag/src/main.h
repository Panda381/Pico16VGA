
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

#define FLAGW	360	// flag width
#define FLAGH	240	// flag height
#define FLAGX	18	// flag X coordinate relative to mast
#define FLAGY	110	// flag Y base coordinate

#define MASTW	24	// mast width
#define MASTH	400	// mast height
#define MASTX	110	// mast X coordinate

// format: 4-bit pixel graphics
// image width: 640 pixels
// image height: 480 lines
// image pitch: 320 bytes
extern const u8 CloudsImg[153600] __attribute__ ((aligned(4)));

// format: 4-bit pixel graphics
// image width: 360 pixels
// image height: 240 lines
// image pitch: 180 bytes
extern const u8 FlagImg[43200] __attribute__ ((aligned(4)));

// format: 4-bit pixel graphics
// image width: 24 pixels
// image height: 400 lines
// image pitch: 12 bytes
extern const u8 MastImg[4800] __attribute__ ((aligned(4)));

#endif // _MAIN_H
