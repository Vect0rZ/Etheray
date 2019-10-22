#ifndef LIGHT_H
#define LIGHT_H

#include "vector3.h"
#include "color3f.h"

typedef struct light
{
	Vec3 origin;
	Vec3 direction;
	float intensity;
	Color3f ambient;
}Light;

/**
 * Creates a new environment light by given
 * origin, direction, ambient color and light intensity.
 * */
Light light_new(Vec3 origin, Vec3 direction, Color3f ambient, float intensity);

#endif