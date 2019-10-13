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

Light create_light(Vec3 origin, Vec3 direction, Color3f ambient, float intensity);

#endif