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

Vec3 obj_surface_normal(Vec3 point, Object *object);
Object obj_create_sphere(Vec3 origin, float radius, Material material);
Object obj_create_plane(Vec3 point, Vec3 normal, Material material);
Object obj_create_triangle(Vec3 v0, Vec3 v1, Vec3 v2, Material material);

#endif