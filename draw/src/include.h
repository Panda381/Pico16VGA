
// ****************************************************************************
//                                 
//                              Common definitions
//
// ****************************************************************************

// ----------------------------------------------------------------------------
//                                   Includes
// ----------------------------------------------------------------------------

#include "../../global.h"	// global common definitions
#include "../vga.pio.h"		// VGA PIO compilation

// images

// format: 4-bit pixel graphics
// image width: 32 pixels
// image height: 32 lines
// image pitch: 16 bytes
extern const u8 Peter4Img[512] __attribute__ ((aligned(4)));

// format: 4-bit pixel graphics
// image width: 32 pixels
// image height: 40 lines
// image pitch: 16 bytes
extern const u8 RPi4Img[640] __attribute__ ((aligned(4)));

// main
#include "main.h"		// main code
