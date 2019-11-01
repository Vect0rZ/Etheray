#include <stdio.h>
#include "color3f.h"
#include "constants.h"

Color3f color3f_new(float r, float g, float b)
{
	Color3f res;
	res.r = r;
	res.g = g;
	res.b = b;
	
	return res;
}

Color3f color3f_mul(Color3f a, Color3f b)
{
	a.r *= b.r;
	a.g *= b.g;
	a.b *= b.b;
	
	return a;
}

Color3f color3f_mul_color(Color3f a, float c)
{
	a.r *= c;
	a.g *= c;
	a.b *= c;
	
	return a;
}

Color3f color3f_add(Color3f a, Color3f b)
{
	a.r += b.r;
	a.g += b.g;
	a.b += b.b;
	
	return a;
}

Color3f color3f_clamp(Color3f a)
{
	if (a.r > 1) a.r = 1;
	if (a.g > 1) a.g = 1;
	if (a.b > 1) a.b = 1;
	
	return a;
}

void color3f_print(Color3f a)
{
	if (!DEBUG)
		return;
	
	printf("R: %f, G: %f, B: %f\n", a.r, a.g, a.b);
}
