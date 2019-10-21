#ifndef COLOR3F_H
#define COLOR3F_H

typedef struct color3f
{
	float r;
	float g;
	float b;
}Color3f;

/**
 * Creates a new color by given RGB components (0.0 - 1.0)
 * */
Color3f color3f_new(float r, float g, float b);
/**
 * Multiplies each component of the two colors.
 * */
Color3f color3f_mul(Color3f a, Color3f b);
/**
 * Multiplies each component of color3f by a given coefficient.
 * */
Color3f color3f_mul_color(Color3f a, float c);
/**
 * Adds the components of the two colors.
 * */
Color3f color3f_add(Color3f a, Color3f b);
/**
 * Clamps the color value between 0 and 1.
 * */
Color3f color3f_clamp(Color3f a);
/**
 * Prints the color in the console.
 * */
void color3f_print(Color3f a);

#endif