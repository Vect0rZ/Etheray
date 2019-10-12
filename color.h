#ifndef COLOR_H
#define COLOR_H

typedef struct color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Color;

Color create_color(unsigned char r, unsigned char g, unsigned char b);

#endif