#include "object.h"

Vec3 surface_normal(Vec3 point, Object *object)
{
	Vec3 normal;
	
	if (object->type == SPHERE)
	{
		normal = sub_vectors(point, object->Obj.sphere.origin);
	}
	else if (object->type == PLANE)
	{
		normal = object->Obj.plane.normal;
	}
	else if (object->type == TRIANGLE)
	{
		/* Assume CCW winding */
		Vec3 e0 = sub_vectors(object->Obj.triangle.v1, object->Obj.triangle.v0);
		Vec3 e1 = sub_vectors(object->Obj.triangle.v2, object->Obj.triangle.v0);
		normal = cross_product(e0, e1);
	}
	
	normalize(&normal);
	return normal;
}

Object create_sphere(Vec3 origin, float radius, Material material)
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

Object create_plane(Vec3 point, Vec3 normal, Material material)
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

Object create_triangle(Vec3 v0, Vec3 v1, Vec3 v2, Material material)
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