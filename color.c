#include "color.h"

Color create_color(unsigned char r, unsigned char g, unsigned char b)
{
	Color res;
	res.r = r;
	res.g = g;
	res.b = b;
	
	return res;
}