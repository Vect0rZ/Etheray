#include <math.h>
#include <stdio.h>
#include "vector3.h"

Vec3 vec3_new(float x, float y, float z)
{
	Vec3 res;
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

float vec3_dot_product(Vec3 a, Vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vec3 vec3_cross_product(Vec3 a, Vec3 b)
{
	Vec3 res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	
	return res;
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
	return vec3_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
	return vec3_new(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_mul(Vec3 a, float coefficient)
{
	return vec3_new(a.x * coefficient, a.y * coefficient, a.z * coefficient);
}

float vec3_distance(Vec3 a, Vec3 b)
{
	return fabs(sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2) + pow((b.z - a.z), 2)));
}

float vec3_magnitude(Vec3 a)
{
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

void vec3_normalize(Vec3 *a)
{
	float len = vec3_magnitude(*a);
	if (len == 0)
	{
		printf("Division by zero\n");
		return;
	}
	a->x /= len;
	a->y /= len;
	a->z /= len;
}

Vec3 vec3_add_delta(Vec3 *point, Vec3 *normal)
{
	/* For refraction? */
	/* if (dot_product(*point, *normal) < 0)
	 	return add_vectors(*point, mul_vector(*normal, 0.001f));
	   else
	 	return sub_vectors(*point, mul_vector(*normal, 0.001f));
	*/
	return vec3_add(*point, vec3_mul(*normal, 0.01f));
}

void vec3_print(Vec3 a)
{
	printf("V: %f, %f, %f;\n",a.x, a.y, a.z);
}

Vec3 vec3_reflect(Vec3 in, Vec3 surface_normal)
{
	Vec3 refl;
	float dot = vec3_dot_product(in, surface_normal);
	Vec3 double_normal = vec3_mul(vec3_mul(surface_normal, 2), dot);
	refl = vec3_sub(in, double_normal);
	vec3_normalize(&refl);
	return refl;
}