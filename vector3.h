#ifndef VECTOR3_H
#define VECTOR3_H

typedef struct vector3
{
	float x;
	float y;
	float z;
} Vec3;

Vec3 create_vec3(float x, float y, float z);
float dot_product(Vec3 a, Vec3 b);
Vec3 cross_product(Vec3 a, Vec3 b);
Vec3 add_vectors(Vec3 a, Vec3 b);
Vec3 sub_vectors(Vec3 a, Vec3 b);
Vec3 mul_vector(Vec3 a, float coefficient);
float dist(Vec3 a, Vec3 b);
float magnitude(Vec3 a);
void normalize(Vec3 *a);
Vec3 fix_point(Vec3 *point, Vec3 *normal);
void print_vector(Vec3 a);
Vec3 reflect(Vec3 in, Vec3 surface_normal);

#endif