#include <math.h>
#include <stdio.h>
#include "vector3.h"

Vec3 create_vec3(float x, float y, float z)
{
	Vec3 res;
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

float dot_product(Vec3 a, Vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vec3 cross_product(Vec3 a, Vec3 b)
{
	Vec3 res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	
	return res;
}

Vec3 add_vectors(Vec3 a, Vec3 b)
{
	return create_vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 sub_vectors(Vec3 a, Vec3 b)
{
	return create_vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 mul_vector(Vec3 a, float coefficient)
{
	return create_vec3(a.x * coefficient, a.y * coefficient, a.z * coefficient);
}

float dist(Vec3 a, Vec3 b)
{
	return fabs(sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2) + pow((b.z - a.z), 2)));
}

float magnitude(Vec3 a)
{
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

void normalize(Vec3 *a)
{
	float len = magnitude(*a);
	if (len == 0)
	{
		printf("Division by zero\n");
		return;
	}
	a->x /= len;
	a->y /= len;
	a->z /= len;
}

Vec3 fix_point(Vec3 *point, Vec3 *normal)
{
	/* For refraction? */
	/* if (dot_product(*point, *normal) < 0)
	 	return add_vectors(*point, mul_vector(*normal, 0.001f));
	   else
	 	return sub_vectors(*point, mul_vector(*normal, 0.001f));
	*/
	return add_vectors(*point, mul_vector(*normal, 0.01f));
}

void print_vector(Vec3 a)
{
	printf("V: %f, %f, %f;\n",a.x, a.y, a.z);
}