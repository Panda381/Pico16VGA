
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

#define BACK_NUM	4		// number of back buffers (height should be divided by this)

// format: 4-bit pixel graphics
// image width: 440 pixels
// image height: 330 lines
// image pitch: 220 bytes
extern const u8 CloudsImg[72600] __attribute__ ((aligned(4)));
#define CLOUDSW	440	// clouds width
#define CLOUDSH	330	// clouds height
#define CLOUDS_SPEEDX 2.0f // clouds speed X
#define CLOUDS_SPEEDY 0.5f // clouds speed Y

// format: 4-bit pixel graphics
// image width: 128 pixels
// image height: 165 lines
// image pitch: 64 bytes
extern const u8 HotairImg[10560] __attribute__ ((aligned(4)));
#define HOTAIRW 128	// hot-air balloon width
#define HOTAIRH	165	// hot-air balloon height
#define HOTAIR_NUM 5	// number of hot-air balloons
#define HOTAIR_MINSPEED 1.5f // hot-air min. move speed
#define HOTAIR_MAXSPEED 5.0f // max. move speed

// format: 4-bit pixel graphics
// image width: 64 pixels
// image height: 145 lines
// image pitch: 32 bytes
extern const u8 BlueImg[4640] __attribute__ ((aligned(4)));
extern const u8 GreenImg[4640] __attribute__ ((aligned(4)));
extern const u8 RedImg[4640] __attribute__ ((aligned(4)));
extern const u8 YellowImg[4640] __attribute__ ((aligned(4)));
#define BALLOONW 64	// balloon width
#define BALLOONH 145	// balloon height
#define BALLOON_NUM 40	// number of party balloons
#define BALLOON_MINSPEED 1.5f // balloon min. speed
#define BALLOON_MAXSPEED 5.0f // balloon max. speed

#endif // _MAIN_H
