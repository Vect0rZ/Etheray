#include "object.h"

Vec3 obj_surface_normal(Vec3 point, Object *object)
{
	Vec3 normal;
	
	if (object->type == SPHERE)
	{
		normal = vec3_sub(point, object->Obj.sphere.origin);
	}
	else if (object->type == PLANE)
	{
		normal = object->Obj.plane.normal;
	}
	else if (object->type == TRIANGLE)
	{
		/* Assume CCW winding */
		Vec3 e0 = vec3_sub(object->Obj.triangle.v1, object->Obj.triangle.v0);
		Vec3 e1 = vec3_sub(object->Obj.triangle.v2, object->Obj.triangle.v0);
		normal = vec3_cross_product(e0, e1);
	}
	
	vec3_normalize(&normal);
	return normal;
}

Object obj_create_sphere(Vec3 origin, float radius, Material material)
{
	Object res;
	
	Sphere sp;
	sp.origin = origin;
	sp.radius = radius;

	res.Obj.sphere = sp;
	res.type = SPHERE;
	res.mat = material;
	return res;
}

Object obj_create_plane(Vec3 point, Vec3 normal, Material material)
{
	Object res;
	Plane plane;
	plane.point = point;
	plane.normal = normal;

	res.Obj.plane = plane;
	res.type = PLANE;
	res.mat = material;
	
	return res;
}

Object obj_create_triangle(Vec3 v0, Vec3 v1, Vec3 v2, Material material)
{
	Object res;
	Triangle triangle;
	triangle.v0 = v0;
	triangle.v1 = v1;
	triangle.v2 = v2;
	res.Obj.triangle = triangle;
	res.type = TRIANGLE;
	res.mat = material;
	
	return res;
}