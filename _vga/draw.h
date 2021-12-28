
// ****************************************************************************
//
//                                   Draw
//
// ****************************************************************************

#ifndef _DRAW_H
#define _DRAW_H

#define TEXTWIDTH	(WIDTH/8)	// width of text buffer (=80)
#define TEXTHEIGHT	(HEIGHT/8)	// height of text buffer (=60)
#define TEXTSIZE	(TEXTWIDTH*TEXTHEIGHT) // size of mono text buffer (=4800)
#define ATEXTSIZE	(TEXTWIDTH*2*TEXTHEIGHT) // size of text buffer with color attributes (=9600)

#define SETPIXEL_EVEN(ddd,ccc) { *(ddd) = (*(ddd) & 0x0f) | ((ccc) << 4); }
#define SETPIXEL_ODD(ddd,ccc) { *(ddd) = (*(ddd) & 0xf0) | (ccc); }
#define SETPIXEL(ddd,xxx,ccc) {					\
	if (((xxx) & 1) == 0)					\
		SETPIXEL_EVEN(ddd,ccc)				\
	else							\
		SETPIXEL_ODD(ddd,ccc); }

#define GETPIXEL_EVEN(sss) (*(sss) >> 4)
#define GETPIXEL_ODD(sss) (*(sss) & 0x0f)
#define GETPIXEL(sss,xxx) ((((xxx) & 1) == 0) ? GETPIXEL_EVEN(sss) : GETPIXEL_ODD(sss))

// back draw frame buffer
extern u8* pDrawBuf;		// pointer to back draw frame buffer
extern int DrawBufY;		// back draw frame buffer Y coordinate
extern int DrawBufH;		// back draw frame buffer height

// convert RGB to pixel color
u8 COLRGB(u8 r, u8 g, u8 b);

// update back fraw frame buffer - copy into main frame buffer
void DrawBufUpdate();

// Draw rectangle
void DrawRect(int x, int y, int w, int h, u8 col);

// Draw frame
void DrawFrame(int x, int y, int w, int h, u8 col);

// clear canvas (fill with black color)
void DrawClear();

// Draw point
void DrawPoint(int x, int y, u8 col);

// get point from frame buffer
u8 GetPoint(int x, int y);

// Draw line
void DrawLine(int x1, int y1, int x2, int y2, u8 col);

// Draw filled circle
void DrawFillCircle(int x0, int y0, int r, u8 col);

// Draw circle
void DrawCircle(int x0, int y0, int r, u8 col);

// Draw text, using system font 8x8 (transparent background)
//  text ... text to print
//  x,y ... destination coordinates
//  sx,sy ... scale font
//  col ... color
void DrawText(const char* text, int x, int y, int sx, int sy, u8 col);

// Draw text with background, using system font 8x8
//  text ... text to print
//  x,y ... destination coordinates
//  sx,sy ... scale font
//  col ... foreground color
//  bgcol ... background color
void DrawTextBg(const char* text, int x, int y, int sx, int sy, u8 col, u8 bgcol);

// Draw text buffer, using system font 8x8 (size TEXTSIZE)
void DrawTextBuf(const char* textbuf, u8 col, u8 bgcol);

// Draw text buffer with color attributes, using system font 8x8 (size ATEXTSIZE)
void DrawATextBuf(const char* textbuf);

// Draw image
//  src = source image
//  xd,yd = destination coordinates
//  xs,ys = source coordinates
//  w,h = draw image dimension
//  ws,hs = source image dimension
void DrawImg(const u8* src, int xd, int yd, int xs, int ys, int w, int h, int ws, int hs);

// Draw image with transparency (col = transparency key color)
//  src = source image
//  xd,yd = destination coordinates
//  xs,ys = source coordinates
//  w,h = draw image dimension
//  ws,hs = source image dimension
//  col = transparency key color
void DrawBlit(const u8* src, int xd, int yd, int xs, int ys, int w, int h, int ws, int hs, u8 col);

// DrawImgMat mode
enum {
	DRAWIMG_WRAP,		// wrap image
	DRAWIMG_NOBORDER,	// no border (transparent border)
	DRAWIMG_CLAMP,		// clamp image (use last pixel as border)
	DRAWING_COLOR,		// color border
	DRAWIMG_TRANSP,		// transparent image with key color
	DRAWIMG_PERSP,		// perspective floor
};

// draw 4-bit image with 2D transformation matrix
//  src ... source image
//  ws ... source image width
//  hs ... source image height
//  x ... destination coordinate X
//  y ... destination coordinate Y
//  w ... destination width
//  h ... destination height
//  m ... transformation matrix (should be prepared using PrepDrawImg or PrepDrawPersp function)
//  mode ... draw mode DRAWIMG_*
//  color ... key or border color (DRAWIMG_PERSP mode: horizon offset)
// Note to wrap and perspective mode: Width and height of source image must be power of 2!
void DrawImgMat(const u8* src, int ws, int hs, int x, int y, int w, int h, const cMat2Df* m, u8 mode, u8 color);

// draw image line interpolated
//  src = source image
//  xd,yd = destination coordinates
//  wd = destination width
//  ws = source width
//  wbs = source line pitch in bytes
void DrawImgLine(const u8* src, int xd, int yd, int wd, int ws, int wbs);

// generate gradient
void GenGrad(u8* dst, int w);

// decode unsigned number into ASCIIZ text buffer (returns number of digits)
//  sep = thousand separator, 0=none
int DecUNum(char* buf, u32 num, char sep = 0);

// decode signed number into ASCIIZ text buffer (returns number of digits)
//  sep = thousand separator, 0=none
int DecNum(char* buf, s32 num, char sep = 0);

// decode hex number (dig = number of digits)
void DecHexNum(char* buf, u32 num, u8 dig);

// decode 2 digits of number
void Dec2Dig(char* buf, u8 num);

// decode 2 digits of number with space character
void Dec2DigSpc(char* buf, u8 num);

// get ascii text length
int TextLen(const char* txt);

#endif // _DRAW_H
