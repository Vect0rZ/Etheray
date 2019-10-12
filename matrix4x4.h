#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "vector3.h"

typedef struct m4x4
{
	float m[4][4];
}M4x4;

M4x4 create_identity_m4x4();
M4x4 create_zero_m4x4();
M4x4 translate_m4x4(Vec3 v);
M4x4 rotate_m4x4(Vec3 v);
M4x4 scale_m4x4(Vec3 v);
M4x4 mul_m4x4(M4x4 m1, M4x4 m2);
Vec3 mul_m4x4_vec3(M4x4 m, Vec3 v);
Vec3 get_basis_vector(M4x4 m, int loc);
void print_matrix(M4x4 m);

#endif