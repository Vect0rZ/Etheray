#ifndef COLOR3F_H
#define COLOR3F_H

typedef struct color3f
{
	float r;
	float g;
	float b;
}Color3f;

Color3f c_col3f(float r, float g, float b);
Color3f mul_c3f(Color3f a, Color3f b);
Color3f mul_c3f_c(Color3f a, float c);
Color3f add_c3f(Color3f a, Color3f b);
Color3f cl_c3f(Color3f a);
void print_c3f(Color3f a);

#endif