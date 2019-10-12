#ifndef OBJECT_H
#define OBJECT_H

#include "enums.h"
#include "plane.h"
#include "sphere.h"
#include "triangle.h"
#include "material.h"
#include "vector3.h"

typedef struct object
{
	union
	{
		Plane plane;
		Sphere sphere;
		Triangle triangle;
	} Obj;
	Material mat;
	Type type;
}Object;

Vec3 surface_normal(Vec3 point, Object *object);

#endif