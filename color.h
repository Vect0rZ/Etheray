#ifndef COLOR_H
#define COLOR_H

typedef struct color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Color;

/**
 * Creates a color by given RGB components (0-255).
 * */
Color color_new(unsigned char r, unsigned char g, unsigned char b);

#endif