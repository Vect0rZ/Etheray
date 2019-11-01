#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "vector3.h"

typedef struct m4x4
{
	float m[4][4];
}M4x4;

M4x4 m4x4_create_identity();
M4x4 m4x4_create_zero();
M4x4 m4x4_translate(Vec3 v);
M4x4 m4x4_rotate(Vec3 v);
M4x4 m4x4_rotate_x(float radians);
M4x4 m4x4_rotate_y(float radians);
M4x4 m4x4_rotate_z(float radians);
M4x4 m4x4_scale(Vec3 v);
M4x4 m4x4_mul(M4x4 m1, M4x4 m2);
Vec3 m4x4_mul_vec3(M4x4 m, Vec3 v);
Vec3 m4x4_get_basis_vector(M4x4 m, int loc);
void m4x4_print(M4x4 m);

#endif
