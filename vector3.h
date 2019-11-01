#ifndef VECTOR3_H
#define VECTOR3_H

typedef struct vector3
{
	float x;
	float y;
	float z;
} Vec3;

Vec3 vec3_new(float x, float y, float z);
float vec3_dot_product(Vec3 a, Vec3 b);
Vec3 vec3_cross_product(Vec3 a, Vec3 b);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, float coefficient);
float vec3_distance(Vec3 a, Vec3 b);
float vec3_magnitude(Vec3 a);
void vec3_normalize(Vec3 *a);
Vec3 vec3_add_delta(Vec3 *point, Vec3 *normal);
void vec3_print(Vec3 a);
Vec3 vec3_reflect(Vec3 in, Vec3 surface_normal);

#endif
