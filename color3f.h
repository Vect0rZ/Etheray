#ifndef COLOR3F_H
#define COLOR3F_H

typedef struct color3f
{
	float r;
	float g;
	float b;
}Color3f;

Color3f color3f_new(float r, float g, float b);
Color3f color3f_mul(Color3f a, Color3f b);
Color3f color3f_mul_color(Color3f a, float c);
Color3f color3f_add(Color3f a, Color3f b);
Color3f color3f_clamp(Color3f a);
void color3f_print(Color3f a);

#endif