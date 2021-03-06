#include "include.h"

// format: 4-bit pixel graphics
// image width: 32 pixels
// image height: 40 lines
// image pitch: 16 bytes
const u8 RPi4Img[640] __attribute__ ((aligned(4))) = {
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
	0xFF, 0x00, 0x00, 0x08, 0x88, 0x80, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0xFF, 
	0xFF, 0x08, 0xAA, 0xAA, 0xAA, 0xAA, 0x80, 0x0F, 0xF0, 0x08, 0xAA, 0xAA, 0xAA, 0xAA, 0x80, 0xFF, 
	0xF0, 0x0A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x0F, 0xF0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0x0F, 
	0xFF, 0x0A, 0xAA, 0xA8, 0x8A, 0xAA, 0xAA, 0xA0, 0x0A, 0xAA, 0xAA, 0xA8, 0x8A, 0xAA, 0xA0, 0xFF, 
	0xFF, 0x08, 0xAA, 0xAA, 0x88, 0x8A, 0xAA, 0xA0, 0x0A, 0xAA, 0xA8, 0x88, 0xAA, 0xAA, 0x80, 0xFF, 
	0xFF, 0x08, 0xAA, 0xAA, 0xAA, 0x80, 0xAA, 0xA0, 0x0A, 0xAA, 0x08, 0xAA, 0xAA, 0xAA, 0x80, 0xFF, 
	0xFF, 0xF0, 0xAA, 0xAA, 0xAA, 0xA8, 0x08, 0x80, 0x08, 0x80, 0x8A, 0xAA, 0xAA, 0xAA, 0x0F, 0xFF, 
	0xFF, 0xF0, 0x0A, 0xAA, 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x0A, 0xAA, 0xAA, 0xAA, 0xA0, 0x0F, 0xFF, 
	0xFF, 0xFF, 0x00, 0xAA, 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x08, 0xAA, 0xAA, 0xAA, 0x00, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xF0, 0x08, 0xAA, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x8A, 0xAA, 0x80, 0x0F, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x84, 0x48, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xF0, 0x00, 0x44, 0x40, 0x0C, 0xCC, 0xCC, 0xC0, 0x04, 0x48, 0x00, 0x0F, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x00, 0xCC, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0xCC, 0xCC, 0x00, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x08, 0xCC, 0xC0, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x0C, 0xCC, 0xC0, 0xFF, 0xFF, 
	0xFF, 0xF0, 0x0C, 0xCC, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0x80, 0x00, 0x00, 0xCC, 0xC0, 0x0F, 0xFF, 
	0xFF, 0xF0, 0x0C, 0xC0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x0C, 0xC8, 0x0F, 0xFF, 
	0xFF, 0xF0, 0x08, 0x00, 0x0C, 0xCC, 0xCC, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x00, 0x80, 0x0F, 0xFF, 
	0xFF, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xC0, 0x04, 0xCC, 0xCC, 0xCC, 0xC0, 0x00, 0x00, 0xFF, 
	0xF0, 0x04, 0x40, 0x0C, 0xCC, 0xCC, 0xCC, 0xC0, 0x0C, 0xCC, 0xCC, 0xCC, 0xC0, 0x0C, 0xC0, 0x0F, 
	0xF0, 0x8C, 0xC0, 0x0C, 0xCC, 0xCC, 0xCC, 0xC0, 0x0C, 0xCC, 0xCC, 0xCC, 0xC8, 0x0C, 0xC4, 0x0F, 
	0x00, 0xCC, 0xC0, 0x8C, 0xCC, 0xCC, 0xCC, 0xC0, 0x0C, 0xCC, 0xCC, 0xCC, 0xC4, 0x0C, 0xCC, 0x00, 
	0x00, 0xCC, 0xC0, 0x4C, 0xCC, 0xCC, 0xCC, 0x40, 0x00, 0xCC, 0xCC, 0xCC, 0xC8, 0x0C, 0xCC, 0x40, 
	0x04, 0xCC, 0xC0, 0x0C, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x8C, 0xCC, 0xCC, 0xC0, 0x0C, 0xCC, 0x80, 
	0x00, 0xCC, 0x80, 0x04, 0xCC, 0xCC, 0xC0, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0x00, 0x08, 0xCC, 0x00, 
	0x00, 0xCC, 0x00, 0x00, 0x08, 0x80, 0x00, 0xCC, 0xCC, 0x40, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x00, 
	0xF0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x8C, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x80, 0x00, 0x0F, 
	0xFF, 0x00, 0x8C, 0xC0, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0xCC, 0xCC, 0x00, 0xFF, 
	0xFF, 0x00, 0xCC, 0xCC, 0x80, 0x08, 0xCC, 0xCC, 0xCC, 0xCC, 0xC0, 0x0C, 0xCC, 0xCC, 0x00, 0xFF, 
	0xFF, 0x00, 0xCC, 0xCC, 0xC4, 0x08, 0xCC, 0xCC, 0xCC, 0xCC, 0x40, 0xCC, 0xCC, 0xCC, 0x00, 0xFF, 
	0xFF, 0xF0, 0xCC, 0xCC, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x0F, 0xFF, 
	0xFF, 0xF0, 0x8C, 0xCC, 0xCC, 0x40, 0x8C, 0xCC, 0xCC, 0xCC, 0x08, 0xCC, 0xCC, 0xC4, 0x0F, 0xFF, 
	0xFF, 0xF0, 0x0C, 0xCC, 0xCC, 0xC0, 0x00, 0xCC, 0xCC, 0x80, 0x0C, 0xCC, 0xCC, 0xC0, 0x0F, 0xFF, 
	0xFF, 0xFF, 0x00, 0xCC, 0xCC, 0x40, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xCC, 0xCC, 0x00, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xF0, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x00, 0x0F, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x4C, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x40, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0C, 0xCC, 0xCC, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x44, 0x48, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};
