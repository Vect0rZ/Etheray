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