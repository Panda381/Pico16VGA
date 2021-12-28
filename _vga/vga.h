
// ****************************************************************************
//
//                                  VGA
//
// ****************************************************************************

#ifndef _VGA_H
#define _VGA_H

#include "config.h"

// VGA Colors
#if COLOR_FORMAT == COLOR_FORMAT_RGB // color format RGB: bit2 red, bit1 green, bit0 blue (recommended)

//	B0 blue
//	B1 green
//	B2 red
//	B3 intensity
#define COL_BLACK	0
#define COL_BLUE	1
#define COL_GREEN	2
#define COL_CYAN	3
#define COL_RED		4
#define COL_MAGENTA	5
#define COL_YELLOW	6
#define COL_GRAY	7
#define COL_DARKGRAY	8
#define COL_LTBLUE	9
#define COL_LTGREEN	10
#define COL_LTCYAN	11
#define COL_LTRED	12
#define COL_LTMAGENTA	13
#define COL_LTYELLOW	14
#define COL_WHITE	15

#elif COLOR_FORMAT == COLOR_FORMAT_BGR // color format BGR: bit2 blue, bit1 green, bit0 red

//	B0 red
//	B1 green
//	B2 blue
//	B3 intensity
#define COL_BLACK	0
#define COL_RED		1
#define COL_GREEN	2
#define COL_YELLOW	3
#define COL_BLUE	4
#define COL_MAGENTA	5
#define COL_CYAN	6
#define COL_GRAY	7
#define COL_DARKGRAY	8
#define COL_LTRED	9
#define COL_LTGREEN	10
#define COL_LTYELLOW	11
#define COL_LTBLUE	12
#define COL_LTMAGENTA	13
#define COL_LTCYAN	14
#define COL_WHITE	15

#else // COLOR_FORMAT == COLOR_FORMAT_GRB, color format GRB: bit2 green, bit1 red, bit0 blue

//	B0 blue
//	B1 red
//	B2 green
//	B3 intensity
#define COL_BLACK	0
#define COL_BLUE	1
#define COL_RED		2
#define COL_MAGENTA	3
#define COL_GREEN	4
#define COL_CYAN	5
#define COL_YELLOW	6
#define COL_GRAY	7
#define COL_DARKGRAY	8
#define COL_LTBLUE	9
#define COL_LTRED	10
#define COL_LTMAGENTA	11
#define COL_LTGREEN	12
#define COL_LTCYAN	13
#define COL_LTYELLOW	14
#define COL_WHITE	15

#endif // COLOR_FORMAT

#define MAXCOL		15	// max color

// display frame buffers
extern ALIGNED u8 FrameBuf[FRAMESIZE];

// initialize VGA (will change system clock to 126 MHz) (auto-initialized at startup)
void VgaInit();

// VGA core
void VgaCore();

// execute remote function on core 1
void Core1Exec(void (*fnc)());

// check if core 1 is busy (executing remote function)
Bool Core1Busy();

// wait if core 1 is busy (executing remote function)
void Core1Wait();

// check VSYNC
Bool IsVSync();

// wait for VSync scanline
void WaitVSync();

#endif // _VGA_H
