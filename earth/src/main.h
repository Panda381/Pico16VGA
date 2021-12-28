
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

// earth image
#define EARTHW	1024	// image width
#define EARTHH	512	// image height

// globe
#define GLOBER	(HEIGHT/2 - 1) // globe radius
#define GLOBEX	(WIDTH/2) // globe X center
#define GLOBEY	(HEIGHT/2) // globe Y center

// format: 4-bit pixel graphics
// image width: 1024 pixels
// image height: 512 lines
// image pitch: 512 bytes
extern const u8 Earth1Img[262144] __attribute__ ((aligned(4)));
extern const u8 Earth2Img[262144] __attribute__ ((aligned(4)));

#endif // _MAIN_H
