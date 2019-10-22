#include <stdio.h>
#include <math.h>

#include "matrix4x4.h"
#include "constants.h"

M4x4 m4x4_create_identity()
{
	M4x4 res;
	res.m[0][0] = 1; res.m[0][1] = 0; res.m[0][2] = 0; res.m[0][3] = 0;
	res.m[1][0] = 0; res.m[1][1] = 1; res.m[1][2] = 0; res.m[1][3] = 0;
	res.m[2][0] = 0; res.m[2][1] = 0; res.m[2][2] = 1; res.m[2][3] = 0;
	res.m[3][0] = 0; res.m[3][1] = 0; res.m[3][2] = 0; res.m[3][3] = 1;
	
	return res;
}

M4x4 m4x4_create_zero()
{
	M4x4 res;
	res.m[0][0] = 0; res.m[0][1] = 0; res.m[0][2] = 0; res.m[0][3] = 0;
	res.m[1][0] = 0; res.m[1][1] = 0; res.m[1][2] = 0; res.m[1][3] = 0;
	res.m[2][0] = 0; res.m[2][1] = 0; res.m[2][2] = 0; res.m[2][3] = 0;
	res.m[3][0] = 0; res.m[3][1] = 0; res.m[3][2] = 0; res.m[3][3] = 0;
	
	return res;
}

M4x4 m4x4_translate(Vec3 v)
{
	M4x4 res = m4x4_create_identity();
	res.m[0][3] = v.x;
	res.m[1][3] = v.y;
	res.m[2][3] = v.z;
	
	return res;
}

M4x4 m4x4_rotate(Vec3 v)
{
	float x_r = v.x * (M_PI / 180);
	float y_r = v.y * (M_PI / 180);
	float z_r = v.z * (M_PI / 180);
	M4x4 x = m4x4_rotate_x(x_r);
	M4x4 y = m4x4_rotate_y(y_r);
	M4x4 z = m4x4_rotate_z(z_r);

	M4x4 res = m4x4_mul(z, m4x4_mul(y, x));
	return res;
}

M4x4 m4x4_rotate_x(float radians)
{
	M4x4 res = m4x4_create_identity();
	res.m[0][0] = 1;
	res.m[1][1] = cosf(radians);
	res.m[1][2] = -sinf(radians);
	res.m[2][1] = sinf(radians);
	res.m[2][2] = cosf(radians);

	return res;
}

M4x4 m4x4_rotate_y(float radians)
{
	M4x4 res = m4x4_create_identity();
	res.m[0][0] = cosf(radians);
	res.m[0][2] = sinf(radians);
	res.m[2][0] = -sinf(radians);
	res.m[2][2] = cosf(radians);
	res.m[1][1] = 1;

	return res;
}

M4x4 m4x4_rotate_z(float radians)
{
	M4x4 res = m4x4_create_identity();
	res.m[0][0] = cosf(radians);
	res.m[0][1] = sinf(radians);
	res.m[1][0] = -sinf(radians);
	res.m[1][1] = cosf(radians);
	res.m[2][2] = 1;

	return res;
}

M4x4 m4x4_scale(Vec3 v)
{
	// TODO: Implement
	return m4x4_create_identity();
}

M4x4 m4x4_mul(M4x4 m1, M4x4 m2)
{
	int i, q, j;
	M4x4 res = m4x4_create_zero();
	/* m1 rows */
	for (i = 0; i < 4; i++)
	{
		/* Walk through the same row 4 times, */ 
		/* also the column index of m2 */
		for (q = 0; q < 4; q++)
		{
			// m1 columns
			for (j = 0; j < 4; j++)
			{
				res.m[i][q] += m1.m[i][j] * m2.m[j][q];
			}
		}
	}
	
	/* 
		The below implementation can be used ONLY for linear transformations 
		Since the translation is affine transformation, we cannot assume
		the columns match with the basis vectors of the transofrmations coordinate system,
		with 1 at the end of each vector.
	*/
	//print_matrix(res);
	//Vec3 x = get_basis_vector(m2, 0);
	//Vec3 y = get_basis_vector(m2, 1);
	//Vec3 z = get_basis_vector(m2, 2);
	//Vec3 w = get_basis_vector(m2, 3);
	//
	///* Transform all of the basis vectors of m2 to the m1 coordinate system (m1 = some transformation) */
	//Vec3 i = mul_m4x4_vec3(m1, x);
	//Vec3 j = mul_m4x4_vec3(m1, y);
	//Vec3 k = mul_m4x4_vec3(m1, z);
	//Vec3 l = mul_m4x4_vec3(m1, w);
    //
	//res.m[0][0] = i.x; res.m[0][1] = j.x; res.m[0][2] = k.x; res.m[0][3] = l.x;
	//res.m[1][0] = i.y; res.m[1][1] = j.y; res.m[1][2] = k.y; res.m[1][3] = l.y;
	//res.m[2][0] = i.z; res.m[2][1] = j.z; res.m[2][2] = k.z; res.m[2][3] = l.z;
	//res.m[3][0] = 0;   res.m[3][1] = 0;	  res.m[3][2] = 0;	 res.m[3][3] = 1;
	
	return res;
}

Vec3 m4x4_mul_vec3(M4x4 m, Vec3 v)
{
	/* 
		Assume last coefficient w is 1, 
		until there is a homogeneous coordinates implementation 
	*/
	Vec3 res;
	res.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * 1;
	res.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * 1;
	res.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * 1;
	/* res.w left out */

	return res;
}

/*
   0 - the first column  (x basis vector)
   1 - the second column (y basis vector)
   2 - the third column  (z basis vector)
*/
Vec3 m4x4_get_basis_vector(M4x4 m, int loc)
{
	Vec3 res;
	res.x = m.m[0][loc];
	res.y = m.m[1][loc];
	res.z = m.m[2][loc];
	
	return res;
}

void m4x4_print(M4x4 m)
{
	if (!DEBUG)
		return;
	
	printf("%f %f %f %f\n", m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	printf("%f %f %f %f\n", m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	printf("%f %f %f %f\n", m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	printf("%f %f %f %f\n", m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
}