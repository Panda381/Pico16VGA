
// ****************************************************************************
//
//                                   Draw
//
// ****************************************************************************

#include "include.h"

// back draw frame buffer
u8* pDrawBuf = FrameBuf;	// pointer to back draw frame buffer
int DrawBufY = 0;		// back draw frame buffer Y coordinate
int DrawBufH = HEIGHT;		// back draw frame buffer height

// convert RGB to pixel color
u8 COLRGB(u8 r, u8 g, u8 b)
{
#if COLOR_FORMAT == COLOR_FORMAT_RGB // color format RGB: bit2 red, bit1 green, bit0 blue (recommended)

	u8 c = (b >> 7) | ((g & 0x80) >> 6) | ((r & 0x80) >> 5);

#elif COLOR_FORMAT == COLOR_FORMAT_BGR // color format BGR: bit2 blue, bit1 green, bit0 red

	u8 c = (r >> 7) | ((g & 0x80) >> 6) | ((b & 0x80) >> 5);

#else // COLOR_FORMAT == COLOR_FORMAT_GRB, color format GRB: bit2 green, bit1 red, bit0 blue

	u8 c = (b >> 7) | ((r & 0x80) >> 6) | ((g & 0x80) >> 5);

#endif // COLOR_FORMAT

	if ((b >= 192) || (g >= 192) || (r >= 192) || (b+g+r >= 220)) c += 8;
	return c;
}

// update back fraw frame buffer - copy into main frame buffer
void DrawBufUpdate()
{
	memcpy(&FrameBuf[DrawBufY*WIDTHBYTE], pDrawBuf, DrawBufH*WIDTHBYTE);
}

// Draw rectangle
void DrawRect(int x, int y, int w, int h, u8 col)
{
	// limit x
	if (x < 0)
	{
		w += x;
		x = 0;
	}

	// limit w
	if (x + w > WIDTH) w = WIDTH - x;
	if (w <= 0) return;

	// limit y
	if (y < DrawBufY)
	{
		h -= DrawBufY - y;
		y = DrawBufY;
	}

	// limit h
	if (y + h > DrawBufY + DrawBufH) h = DrawBufY + DrawBufH - y;
	if (h <= 0) return;

	// draw
	u8* d0 = &pDrawBuf[x/2 + (y-DrawBufY)*WIDTHBYTE];
	u8* d;
	int i;
	u8 col2 = col | (col << 4);
	for (; h > 0; h--)
	{
		d = d0;
		i = w;

		if ((x & 1) != 0)
		{
			SETPIXEL_ODD(d,col);
			d++;
			i--;
		}

		for (; i > 1; i -= 2) *d++ = col2;

		if (i > 0) SETPIXEL_EVEN(d,col);

		d0 += WIDTHBYTE;
	}
}

// Draw frame
void DrawFrame(int x, int y, int w, int h, u8 col)
{
	if ((w <= 0) || (h <= 0)) return;
	DrawRect(x, y, w-1, 1, col);
	DrawRect(x+w-1, y, 1, h-1, col);
	DrawRect(x+1, y+h-1, w-1, 1, col);
	DrawRect(x, y+1, 1, h-1, col);
}

// clear canvas (fill with black color)
void DrawClear()
{
	memset(pDrawBuf, COL_BLACK | (COL_BLACK << 4), DrawBufH*WIDTHBYTE);
}

// Draw point
void DrawPoint(int x, int y, u8 col)
{
	y -= DrawBufY;

	// check coordinates
	if (((u32)x >= (u32)WIDTH) || ((u32)y >= (u32)DrawBufH)) return;

	// draw pixel
	u8* d = &pDrawBuf[x/2 + y*WIDTHBYTE];
	SETPIXEL(d,x,col);
}

// get point from frame buffer
u8 GetPoint(int x, int y)
{
	// check coordinates
	if (((u32)x >= (u32)WIDTH) || ((u32)y >= (u32)HEIGHT)) return COL_BLACK;

	// get pixel
	u8* s = &FrameBuf[x/2 + y*WIDTHBYTE];
	return GETPIXEL(s,x);
}

// Draw line
void DrawLine(int x1, int y1, int x2, int y2, u8 col)
{
	// Y relative to draw buffer begin
	y1 -= DrawBufY;
	y2 -= DrawBufY;

	// difference of coordinates
	int dx = x2 - x1;
	int dy = y2 - y1;

	// increment X
	int sx = 1;
	if (dx < 0)
	{
		sx = -1;
		dx = -dx;
	}

	// increment Y
	int sy = 1;
	int ddy = WIDTHBYTE;
	if (dy < 0)
	{
		sy = -1;
		ddy = -ddy;
		dy = -dy;
	}

	// destination address
	u8* d0 = &pDrawBuf[y1*WIDTHBYTE];
	u8* d;

	// steeply in X direction, X is prefered as base
	if (dx > dy)
	{
		int m = 2*dy;
		int p = m - dx;
		dx = 2*dx;

		// slow mode, check coordinates
		if (((u32)x1 >= (u32)WIDTH) || ((u32)x2 >= (u32)WIDTH) ||
			((u32)y1 >= (u32)DrawBufH) || ((u32)y2 >= (u32)DrawBufH))
		{
			x2 += sx;
			for (; x1 != x2; x1 += sx)
			{
				if (((u32)x1 < (u32)WIDTH) && ((u32)y1 < (u32)DrawBufH))
				{
					d = &d0[x1/2];
					SETPIXEL(d,x1,col);
				}

				if (p > 0)
				{
					d0 += ddy;
					y1 += sy;
					p -= dx;
				}
				p += m;
			}
		}

		// fast mode, do not check coordinates
		else
		{
			x2 += sx;
			for (; x1 != x2; x1 += sx)
			{
				d = &d0[x1/2];
				SETPIXEL(d,x1,col);

				if (p > 0)
				{
					d0 += ddy;
					p -= dx;
				}
				p += m;
			}
		}
	}

	// steeply in Y direction, Y is prefered as base
	else
	{
		int m = 2*dx;
		int p = m - dy;
		dy = 2*dy;

		// slow mode, check coordinates
		if (((u32)x1 >= (u32)WIDTH) || ((u32)x2 >= (u32)WIDTH) ||
			((u32)y1 >= (u32)DrawBufH) || ((u32)y2 >= (u32)DrawBufH))
		{
			y2 += sy;
			for (; y1 != y2; y1 += sy)
			{
				if (((u32)x1 < (u32)WIDTH) && ((u32)y1 < (u32)DrawBufH))
				{
					d = &d0[x1/2];
					SETPIXEL(d,x1,col);
				}

				if (p > 0)
				{
					x1 += sx;
					p -= dy;
				}
				p += m;
				d0 += ddy;
			}
		}

		// fast mode, do not check coordinates
		else
		{
			y2 += sy;
			for (; y1 != y2; y1 += sy)
			{
				d = &d0[x1/2];
				SETPIXEL(d,x1,col);

				if (p > 0)
				{
					x1 += sx;
					p -= dy;
				}
				p += m;
				d0 += ddy;
			}
		}
	}
}

// Draw filled circle
void DrawFillCircle(int x0, int y0, int r, u8 col)
{
	int x, y;
	if (r <= 0) return;
	int r2 = r*(r-1);
	r--;

	// full circle
	for (y = -r; y <= r; y++)
	{
		for (x = -r; x <= r; x++)
		{
			if ((x*x + y*y) <= r2) DrawPoint(x+x0, y+y0, col);
		}
	}
}

// Draw circle
void DrawCircle(int x0, int y0, int r, u8 col)
{
	int x, y;
	if (r <= 0) return;
	r--;

	x = 0;
	y = r;
	int p = 1 - r;

	while (x <= y)
	{
		DrawPoint(x0+y, y0-x, col);
		DrawPoint(x0+x, y0-y, col);
		DrawPoint(x0-x, y0-y, col);
		DrawPoint(x0-y, y0-x, col);
		DrawPoint(x0-y, y0+x, col);
		DrawPoint(x0-x, y0+y, col);
		DrawPoint(x0+x, y0+y, col);
		DrawPoint(x0+y, y0+x, col);

		x++;
		if (p > 0)
		{
			y--;
			p -= 2*y;
		}
		p += 2*x + 1;
	}
}

// Draw text, using system font 8x8 (transparent background)
//  text ... text to print
//  x,y ... destination coordinates
//  sx,sy ... scale font
//  col ... color
void DrawText(const char* text, int x, int y, int sx, int sy, u8 col)
{
	// prepare
	int x0 = x;
	int y0 = y;
	u8 ch;
	int i, j, k, m;
	const u8* s;

	// loop through characters of text
	for (;;)
	{
		// get next character of the text
		ch = (u8)*text++;
		if (ch == 0) break;

		// prepare pointer to font sample
		s = &Font8x8[ch];

		// loop through lines of one character
		y = y0;
		for (i = 8; i > 0; i--)
		{
			// get one font sample
			ch = *s;
			s += 256;

			// loop through pixels of one character line
			x = x0;
			for (j = 8; j > 0; j--)
			{
				// pixel is set
				if ((ch & 0x80) != 0)
				{
					for (k = 0; k < sx; k++)
						for (m = 0; m < sy; m++)
							DrawPoint(x+k, y+m, col);
				}
				x += sx;
				ch <<= 1;
			}
			y += sy;
		}

		// shift to next character position
		x0 += 8*sx;
	}
}

// Draw text with background, using system font 8x8
//  text ... text to print
//  x,y ... destination coordinates
//  sx,sy ... scale font
//  col ... foreground color
//  bgcol ... background color
void DrawTextBg(const char* text, int x, int y, int sx, int sy, u8 col, u8 bgcol)
{
	// prepre
	int x0 = x;
	int y0 = y;
	u8 ch, c;
	int i, j, k, m;
	const u8* s;

	// loop through characters of text
	for (;;)
	{
		// get next character of the text
		ch = (u8)*text++;
		if (ch == 0) break;

		// prepare pointer to font sample
		s = &Font8x8[ch];

		// loop through lines of one character
		y = y0;
		for (i = 8; i > 0; i--)
		{
			// get one font sample
			ch = *s;
			s += 256;

			// loop through pixels of one character line
			x = x0;
			for (j = 8; j > 0; j--)
			{
				// pixel is set
				c = ((ch & 0x80) != 0) ? col : bgcol;

				// draw pixel
				for (k = 0; k < sx; k++)
					for (m = 0; m < sy; m++)
						DrawPoint(x+k, y+m, c);

				x += sx;
				ch <<= 1;
			}
			y += sy;
		}

		// shift to next character position
		x0 += 8*sx;
	}
}

// Draw text buffer, using system font 8x8 (size TEXTSIZE)
void DrawTextBuf(const char* textbuf, u8 col, u8 bgcol)
{
	// prepare
	u8 ch, c;
	int num, i, j;
	const u8* s;
	int y0, x0;
	u8* d;
	u8 b;

	// loop through characters of text
	for (y0 = 0; y0 < HEIGHT; y0 += 8)
	{
		d = &pDrawBuf[(y0-DrawBufY)*WIDTHBYTE];

		for (x0 = 0; x0 < WIDTH; x0 += 8)
		{
			// get next character of the text
			ch = (u8)*textbuf++;

			// prepare pointer to font sample
			s = &Font8x8[ch];

			// loop through lines of one character
			for (i = 8; i > 0; i--)
			{
				// get one font sample
				ch = *s;
				s += 256;

				// Y is out of buffer
				if ((y0 + 8 - i < DrawBufY) || (y0 + 8 - i >= DrawBufY + DrawBufH))
				{
					d += 4;
				}
				else
				{
					// loop through pixels of one character line
					for (j = 4; j > 0; j--)
					{
						// draw 2 pixels
						b = (((ch & 0x80) != 0) ? col : bgcol) << 4;
						b |= ((ch & 0x40) != 0) ? col : bgcol;
						*d++ = b;
						ch <<= 2;
					}
				}
				d += WIDTHBYTE - 4;
			}
			d += 4 - 8*WIDTHBYTE;
		}
	}
}

// Draw text buffer with color attributes, using system font 8x8 (size ATEXTSIZE)
void DrawATextBuf(const char* textbuf)
{
	// prepre
	u8 ch, c, col, b, col2, bgcol;
	int num, i, j;
	const u8* s;
	int y0, x0;
	u8* d;

	// loop through characters of text
	for (y0 = 0; y0 < HEIGHT; y0 += 8)
	{
		d = &pDrawBuf[(y0-DrawBufY)*WIDTHBYTE];

		for (x0 = 0; x0 < WIDTH; x0 += 8)
		{
			// get next character of the text
			ch = (u8)*textbuf++;

			// get color attributes
			col2 = (u8)*textbuf++;
			col = col2 & 0x0f;
			bgcol = col2 >> 4;

			// prepare pointer to font sample
			s = &Font8x8[ch];

			// loop through lines of one character
			for (i = 8; i > 0; i--)
			{
				// get one font sample
				ch = *s;
				s += 256;

				// Y is out of buffer
				if ((y0 + 8 - i < DrawBufY) || (y0 + 8 - i >= DrawBufY + DrawBufH))
				{
					d += 4;
				}
				else
				{
					// loop through pixels of one character line
					for (j = 4; j > 0; j--)
					{
						// draw 2 pixels
						b = (((ch & 0x80) != 0) ? col : bgcol) << 4;
						b |= ((ch & 0x40) != 0) ? col : bgcol;
						*d++ = b;
						ch <<= 2;
					}
				}
				d += WIDTHBYTE - 4;
			}
			d += 4 - 8*WIDTHBYTE;
		}
	}
}

// Draw image (or cut of it)
//  src = source image
//  xd,yd = destination coordinates
//  xs,ys = source coordinates
//  w,h = draw image dimension
//  ws,hs = source image dimension
void DrawImg(const u8* src, int xd, int yd, int xs, int ys, int w, int h, int ws, int hs)
{
	// Y relative to draw buffer begin
	yd -= DrawBufY;

	// limit coordinate X
	if (xd < 0)
	{
		w += xd;
		xs += -xd;
		xd = 0;
	}

	if (xs < 0)
	{
		w += xs;
		xd += -xs;
		xs = 0;
	}

	// limit w
	if (xd + w > WIDTH) w = WIDTH - xd;
	if (xs + w > ws) w = ws - xs;
	if (w <= 0) return;

	// limit coordinate Y
	if (yd < 0)
	{
		h += yd;
		ys += -yd;
		yd = 0;
	}

	if (ys < 0)
	{
		h += ys;
		yd += -ys;
		ys = 0;
	}

	// limit h
	if (yd + h > DrawBufH) h = DrawBufH - yd;
	if (ys + h > hs) h = hs - ys;
	if (h <= 0) return;

	// draw image
	ws /= 2;
	u8* d0 = &pDrawBuf[xd/2 + yd*WIDTHBYTE];
	const u8* s0 = &src[xs/2 + ys*ws];
	int i;
	u8* d;
	const u8* s;
	int xs2, xd2;
	for (; h > 0; h--)
	{
		d = d0;
		s = s0;
		i = w;
		xs2 = xs;
		xd2 = xd;

		// odd destination
		if ((xd2 & 1) != 0)
		{
			if ((xs2 & 1) != 0)
			{
				// odd source
				*d = (*d & 0xf0) | (*s & 0x0f);
				s++;
			}
			else
				// even source
				*d = (*d & 0xf0) | (*s >> 4);
			d++;
			xd2++;
			xs2++;
			i--;
		}

		// destination is now even
		if ((xs2 & 1) != 0)
		{
			// odd source
			for (; i > 1; i -= 2)
			{
				*d = (*s << 4) | (s[1] >> 4);
				d++;
				s++;
				xd2 += 2;
				xs2 += 2;
			}
		}
		else
		{
			// even source
			for (; i > 1; i -= 2)
			{
				*d = *s;
				d++;
				s++;
				xd2 += 2;
				xs2 += 2;
			}
		}

		// if last pixel left - destination is now even
		if (i > 0)
		{
			if ((xs2 & 1) != 0)
				// odd source
				*d = (*d & 0x0f) | (*s << 4);
			else
				// even source
				*d = (*d & 0x0f) | (*s & 0xf0);
		}

		d0 += WIDTHBYTE;
		s0 += ws;
	}
}

// Draw image with transparency
//  src = source image
//  xd,yd = destination coordinates
//  xs,ys = source coordinates
//  w,h = draw image dimension
//  ws,hs = source image dimension
//  col = transparency key color
void DrawBlit(const u8* src, int xd, int yd, int xs, int ys, int w, int h, int ws, int hs, u8 col)
{
	// Y relative to draw buffer begin
	yd -= DrawBufY;

	// limit coordinate X
	if (xd < 0)
	{
		w += xd;
		xs += -xd;
		xd = 0;
	}

	if (xs < 0)
	{
		w += xs;
		xd += -xs;
		xs = 0;
	}

	// limit w
	if (xd + w > WIDTH) w = WIDTH - xd;
	if (xs + w > ws) w = ws - xs;
	if (w <= 0) return;

	// limit coordinate Y
	if (yd < 0)
	{
		h += yd;
		ys += -yd;
		yd = 0;
	}

	if (ys < 0)
	{
		h += ys;
		yd += -ys;
		ys = 0;
	}

	// limit h
	if (yd + h > DrawBufH) h = DrawBufH - yd;
	if (ys + h > hs) h = hs - ys;
	if (h <= 0) return;

	// draw image
	ws /= 2;
	u8* d0 = &pDrawBuf[xd/2 + yd*WIDTHBYTE];
	const u8* s0 = &src[xs/2 + ys*ws];
	int i;
	u8 c;
	u8* d;
	const u8* s;
	int xs2, xd2;
	u8 col2 = col << 4;
	u8 b, b2;
	for (; h > 0; h--)
	{
		d = d0;
		s = s0;
		i = w;
		xs2 = xs;
		xd2 = xd;

		// odd destination
		if ((xd2 & 1) != 0)
		{
			if ((xs2 & 1) != 0)
			{
				// odd source
				c = (*s & 0x0f);
				if (c != col2) *d = (*d & 0xf0) | c;
				s++;
			}
			else
			{
				// even source
				c = (*s >> 4);
				if (c != col) *d = (*d & 0xf0) | c;
			}
			d++;
			xd2++;
			xs2++;
			i--;
		}

		// destination is now even
		if ((xs2 & 1) != 0)
		{
			// odd source
			for (; i > 1; i -= 2)
			{
				b = *d;			
				c = *s & 0x0f;
				if (c != col) b = (b & 0x0f) | (c << 4);
				c = s[1] >> 4;
				if (c != col) b = (b & 0xf0) | c;
				*d = b;
				d++;
				s++;
				xd2 += 2;
				xs2 += 2;
			}
		}
		else
		{
			// even source
			for (; i > 1; i -= 2)
			{
				b = *d;
				b2 = *s;
				c = b2 & 0xf0;
				if (c != col2) b = (b & 0x0f) | c;
				c = b2 & 0x0f;
				if (c != col) b = (b & 0xf0) | c;
				*d = b;
				d++;
				s++;
				xd2 += 2;
				xs2 += 2;
			}
		}

		// if last pixel left - destination is now even
		if (i > 0)
		{
			if ((xs2 & 1) != 0)
			{
				// odd source
				c = *s & 0x0f;
				if (c != col) *d = (*d & 0x0f) | (c << 4);
			}
			else
			{
				// even source
				c = *s & 0xf0;
				if (c != col2) *d = (*d & 0x0f) | c;
			}
		}

		d0 += WIDTHBYTE;
		s0 += ws;
	}
}

// draw 8-bit image with 2D transformation matrix
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
void DrawImgMat(const u8* src, int ws, int hs, int x, int y, int w, int h, const cMat2Df* m, u8 mode, u8 color)
{
	// Y relative to draw buffer begin
	y -= DrawBufY;

	// limit x
	int x0 = -w/2; // start X coordinate
	if (x < 0)
	{
		w += x;
		x0 -= x;
		x = 0;
	}
	if (x + w > WIDTH) w = WIDTH - x;
	if (w <= 0) return;

	// limit y
	int h0 = h;
	int y0 = (mode == DRAWIMG_PERSP) ? (-h) : (-h/2); // start Y coordinate
	y0 -= DrawBufY;
	if (y < 0)
	{
		h += y;
		y0 -= y;
		y = 0;
	}
	if (y + h > DrawBufH) h = DrawBufH - y;
	if (h <= 0) return;

	// load transformation matrix and convert to integer fractional number
	int m11 = (int)(m->m11*FRACTMUL+0.5f);
	int m12 = (int)(m->m12*FRACTMUL+0.5f);
	int m13 = (int)(m->m13*FRACTMUL+0.5f);
	int m21 = (int)(m->m21*FRACTMUL+0.5f);
	int m22 = (int)(m->m22*FRACTMUL+0.5f);
	int m23 = (int)(m->m23*FRACTMUL+0.5f);

	// zero size image
	if ((m11 == 0) || (m22 == 0)) return;

	// prepare variables
	int xy0m, yy0m; // temporary Y members
	u8* d = &pDrawBuf[WIDTHBYTE*y]; // destination image
	int i, xx, x2, y2;
	const u8* s;
	ws /= 2;

	// wrap image
	if (mode == DRAWIMG_WRAP)
	{
		// coordinate mask
		int xmask = ws - 1;
		int ymask = hs - 1;

		for (; h > 0; h--)
		{
			xx = x;
			xy0m = x0*m11 + y0*m12 + m13;
			yy0m = x0*m21 + y0*m22 + m23;
		
			for (i = w; i > 0; i--)
			{
				x2 = (xy0m>>FRACT) & xmask;
				y2 = (yy0m>>FRACT) & ymask;
				s = &src[y2*ws];
				SETPIXEL(&d[xx/2],xx,GETPIXEL(&s[x2/2],x2));
				xy0m += m11; // x0*m11
				yy0m += m21; // x0*m21
				xx++;
			}

			y0++;
			d += WIDTHBYTE;
		}
	}

	// no border
	else if (mode == DRAWIMG_NOBORDER)
	{
		// source image dimension
		u32 ww = ws;
		u32 hh = hs;

		for (; h > 0; h--)
		{
			xx = x;
			xy0m = x0*m11 + y0*m12 + m13;
			yy0m = x0*m21 + y0*m22 + m23;

			for (i = w; i > 0; i--)
			{
				x2 = xy0m>>FRACT;
				y2 = yy0m>>FRACT;
				if (((u32)x2 < ww) && ((u32)y2 < hh))
				{
					s = &src[y2*ws];
					SETPIXEL(&d[xx/2],xx,GETPIXEL(&s[x2/2],x2));
				}
				xy0m += m11; // x0*m11
				yy0m += m21; // x0*m21
				xx++;
			}
			y0++;
			d += WIDTHBYTE;
		}
	}

	// clamp image
	else if (mode == DRAWIMG_CLAMP)
	{
		// source image dimension
		u32 ww = ws - 1;
		u32 hh = hs - 1;

		for (; h > 0; h--)
		{
			xx = x;
			xy0m = x0*m11 + y0*m12 + m13;
			yy0m = x0*m21 + y0*m22 + m23;
		
			for (i = w; i > 0; i--)
			{
				x2 = xy0m>>FRACT;
				y2 = yy0m>>FRACT;
				if (x2 < 0) x2 = 0;
				if (x2 > ww) x2 = ww;
				if (y2 < 0) y2 = 0;
				if (y2 > hh) y2 = hh;
				s = &src[y2*ws];
				SETPIXEL(&d[xx/2],xx,GETPIXEL(&s[x2/2],x2));
				xy0m += m11; // x0*m11
				yy0m += m21; // x0*m21
				xx++;
			}
			y0++;
			d += WIDTHBYTE;
		}
	}

	// color border
	else if (mode == DRAWING_COLOR)
	{
		// source image dimension
		u32 ww = ws;
		u32 hh = hs;

		for (; h > 0; h--)
		{
			xx = x;
			xy0m = x0*m11 + y0*m12 + m13;
			yy0m = x0*m21 + y0*m22 + m23;
		
			for (i = w; i > 0; i--)
			{
				x2 = xy0m>>FRACT;
				y2 = yy0m>>FRACT;
				if (((u32)x2 < ww) && ((u32)y2 < hh))
				{
					s = &src[y2*ws];
					SETPIXEL(&d[xx/2],xx,GETPIXEL(&s[x2/2],x2));
				}
				else
				{
					SETPIXEL(&d[xx/2],xx,color);
				}
				xy0m += m11; // x0*m11
				yy0m += m21; // x0*m21
				xx++;
			}
			y0++;
			d += WIDTHBYTE;
		}
	}

	// transparency
	else if (mode == DRAWIMG_TRANSP)
	{
		u32 ww = ws;
		u32 hh = hs;
		u8 c;

		for (; h > 0; h--)
		{
			xx = x;
			xy0m = x0*m11 + y0*m12 + m13;
			yy0m = x0*m21 + y0*m22 + m23;
		
			for (i = w; i > 0; i--)
			{
				x2 = xy0m>>FRACT;
				y2 = yy0m>>FRACT;
				if (((u32)x2 < ww) && ((u32)y2 < hh))
				{
					s = &src[y2*ws];
					c = GETPIXEL(&s[x2/2],x2);
					if (c != color) SETPIXEL(&d[xx/2],xx,c);
				}
				xy0m += m11; // x0*m11
				yy0m += m21; // x0*m21
				xx++;
			}
			y0++;
			d += WIDTHBYTE;
		}
	}

	// perspective
	else if (mode == DRAWIMG_PERSP)
	{
		// coordinate mask
		int xmask = ws - 1;
		int ymask = hs - 1;

		for (; h > 0; h--)
		{
			xx = x;
			int dist = FRACTMUL*h0/(h0 + y0 + color + 1);
			int m11b = (m11*dist)>>FRACT;
			int m21b = (m21*dist)>>FRACT;
			int m12b = (m12*dist)>>FRACT;
			int m22b = (m22*dist)>>FRACT;

			xy0m = x0*m11b + y0*m12b + m13;
			yy0m = x0*m21b + y0*m22b + m23;
		
			for (i = w; i > 0; i--)
			{
				x2 = (xy0m>>FRACT) & xmask;
				xy0m += m11b; // x0*m11

				y2 = (yy0m>>FRACT) & ymask;
				yy0m += m21b; // x0*m21

				s = &src[y2*ws];
				SETPIXEL(&d[xx/2],xx,GETPIXEL(&s[x2/2],x2));
				xx++;
			}
			y0++;
			d += WIDTHBYTE;
		}
	}
}

// draw image line interpolated
//  src = source image
//  xd,yd = destination coordinates
//  wd = destination width
//  ws = source width
//  wbs = source line pitch in bytes
void DrawImgLine(const u8* src, int xd, int yd, int wd, int ws, int wbs)
{
	// Y relative to draw buffer begin
	yd -= DrawBufY;

	// some base checks
	if ((wd <= 0) || (ws <= 0) || (yd < 0) || (yd >= DrawBufH)) return;

	// pixel increment
	int dinc = FRACTMUL*ws/wd;

	// prepare buffers
	u8* d = &pDrawBuf[yd*WIDTHBYTE]; // destination address
	int i, j;
	int xs = 0;
	int dadd = 0;
	for (i = 0; i < wd; i++)
	{
		SETPIXEL(&d[xd/2],xd,GETPIXEL(&src[xs/2],xs));
		dadd += dinc;
		xs += dadd >> FRACT;
		dadd &= (FRACTMUL-1);
		xd++;
	}
}

// rainbow gradient table
u8 RainbowGrad[] = {
	COL_RED,		// red
	COL_LTRED,		// light red
	COL_LTYELLOW,		// light yellow
	COL_YELLOW,		// yellow
	COL_GREEN,		// green
	COL_LTGREEN,		// light green
	COL_LTCYAN,		// light cyan
	COL_CYAN,		// cyan
	COL_BLUE,		// blue
	COL_LTBLUE,		// light blue
	COL_LTMAGENTA,		// light magenta
	COL_MAGENTA,		// magenta
};

#define RAINBOW_NUM count_of(RainbowGrad)

// generate gradient (must start of even X coordinate)
void GenGrad(u8* dst, int w)
{
	// sample - rainbow pattern
	int i;
	for (i = 0; i < w; i++)
	{
		if ((i & 1) == 0)
		{
			*dst = (*dst & 0x0f) | (RainbowGrad[i*RAINBOW_NUM/w] << 4);
		}
		else
		{
			*dst = (*dst & 0xf0) | RainbowGrad[i*RAINBOW_NUM/w];
			dst++;
		}
	}
}

// decode unsigned number into ASCIIZ text buffer (returns number of digits)
//  sep = thousand separator, 0=none
int DecUNum(char* buf, u32 num, char sep /* = 0 */)
{
	// temporary buffer (max. 10 digits + 3 separators + 1 NUL)
	char b[14];

	// decode number
	char* d = &b[13];
	*d = 0;
	int n = 0;
	u32 k;
	int sepn = 0;
	do {
		// thousand separator
		if ((sepn == 3) && (sep != 0))
		{
			sepn = 0;
			d--;
			*d = sep;
			n++;
		}

		k = num / 10;
		num -= k*10;
		d--;
		*d = (char)(num + '0');
		num = k;
		n++;
		sepn++;
	} while (num != 0);

	// copy number to destination buffer
	memcpy(buf, d, n+1);
	return n;
}

// decode signed number into ASCIIZ text buffer (returns number of digits)
//  sep = thousand separator, 0=none
int DecNum(char* buf, s32 num, char sep /* = 0 */)
{
	if (num >= 0) return DecUNum(buf, num, sep);

	num = -num;
	*buf++ = '-';
	return DecUNum(buf, num, sep) + 1;
}

// decode hex number (dig = number of digits)
void DecHexNum(char* buf, u32 num, u8 dig)
{
	buf += dig;
	*buf = 0;
	u8 ch;

	for (; dig > 0; dig--)
	{
		buf--;
		ch = (num & 0x0f) + '0';
		if (ch > '9') ch += 'A' - ('9'+1);
		*buf = ch;
		num >>= 4;
	}
}

// decode 2 digits of number
void Dec2Dig(char* buf, u8 num)
{
	u8 n = num/10;
	*buf++ = (char)(n + '0');
	num -= n*10;
	*buf++ = (char)(num + '0');
	*buf = 0;
}

// decode 2 digits of number with space character
void Dec2DigSpc(char* buf, u8 num)
{
	u8 n = num/10;
	if (n == 0)
		*buf++ = ' ';
	else
		*buf++ = (char)(n + '0');
	num -= n*10;
	*buf++ = (char)(num + '0');
	*buf = 0;
}

// get ascii text length
int TextLen(const char* txt)
{
	const char* t = txt;
	while (*t != 0) t++;
	return t - txt;
}
