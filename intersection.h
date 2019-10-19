#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vector3.h"
#include "object.h"
#include "color3f.h"

typedef struct intersection_data
{
	int has_intersection;
	float distance;
	Vec3 point;
	Object obj;
	Color3f col;
}Intersection;

#endif