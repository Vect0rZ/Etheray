#include "color.h"

Color color_new(unsigned char r, unsigned char g, unsigned char b)
{
	Color res;
	res.r = r;
	res.g = g;
	res.b = b;
	
	return res;
}