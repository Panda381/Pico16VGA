
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

#define IMGWIDTH 256		// image width
#define IMGHEIGHT 256		// image height
#define IMGWIDTHBYTE 128	// bytes per line of image
#define IMGSIZE (IMGWIDTHBYTE*IMGHEIGHT) // image size

#define TWISTW	128		// twister width

// Images (size 32 KB)
// format: 4-bit pixel graphics
// image width: 256 pixels
// image height: 256 lines
// image pitch: 128 bytes
extern const u8 CobbleStoneImg[32768] __attribute__ ((aligned(4)));
extern const u8 FabricImg[32768] __attribute__ ((aligned(4)));
extern const u8 SoilImg[32768] __attribute__ ((aligned(4)));
extern const u8 WoodImg[32768] __attribute__ ((aligned(4)));

#endif // _MAIN_H
