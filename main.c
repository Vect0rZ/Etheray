#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "enums.h"
#include "constants.h"
#include "vector3.h"
#include "object.h"
#include "matrix4x4.h"
#include "material.h"
#include "color3f.h"
#include "bmp16.h"
#include "camera.h"
#include "light.h"
#include "intersection.h"

typedef struct ray
{
	Vec3 origin;
	Vec3 direction;
} Ray;

/* Raytracing methods */
Ray pixel_trace(Camera *camera, int i, int j, float bx, float by);
Ray generate_pixel_ray(SamplingMethod method, Camera* camera, int i, int j, int maxRays, int currentRay);
Intersection ray_sphere_intersection(Ray *ray, Sphere *sphere);
Intersection ray_plane_intersection(Ray *ray, Plane *plane);
Intersection ray_triangle_intersection(Ray *ray, Triangle *triangle);
Intersection intersects(Ray *ray, Object *objects, int count);
Intersection ray_trace(Ray *ray, Object *objects, int count, Light *light, int depth);
Ray reflection_ray(Vec3 point, Vec3 normal);
/* Light methods */
Color3f ambient(Ray *ray, Intersection *intersection);
Color3f phong(Ray *ray, Intersection *intersection, Light *light, Object* objects, int count);
Color3f reflection(Ray *ray, Intersection *intersection, Object *objects, int count, Light* light, int depth);
/* Material methods */

int main(int argc, char** argv)
{
	int width = 1280;
	int height = 720;
	int n_objects = 5;
	Object *objects = malloc(sizeof(Object) * n_objects);
	
	Camera camera = camera_new(vec3_new(0, 0, 0), 70, 1, width, height);
	Object s1 = obj_create_sphere(vec3_new(2, 1, -15), 2,
							  material_new(REFLECTIVE,
									color3f_new(0.811, 0.988, 0.454), 
									color3f_new(0.36, 0.36, 0.36), 
									color3f_new(0.7, 0.7, 0.7),
									20, 0.7));
	Object s2 = obj_create_sphere(vec3_new(-6, 0, -15), 4,
							  material_new(REFLECTIVE,
								color3f_new(0.282, 0.376, 0.498), 
								color3f_new(0.7, 0.7, 0.7), 
								color3f_new(0.2, 0.2, 0.2),
								10, 0.07));
	Object s3 = obj_create_sphere(vec3_new(2.5, 2, -7), 1.2, 
							  material_new(REFLECTIVE,
									color3f_new(1, 0.223, 0.301), 
									color3f_new(0.1, 0.1, 0.1), 
									color3f_new(0.36, 0.36, 0.36),
									20, 0.4));
	/* Plane located on the origin, and with normal pointing UP. (XZ plane) */
	Object p1 = obj_create_plane(vec3_new(0, -4, 0), vec3_new(0, 1, 0),
							 material_new(REFLECTIVE,
								   color3f_new(0.1, 0.1, 0.1), 
								   color3f_new(0.1, 0.1, 0.1), 
								   color3f_new(0.1, 0.1, 0.1),
								   7, 0.2));
	 Object t1 = obj_create_triangle(vec3_new(-7, 4, -15), vec3_new(7, 4, -15), vec3_new(0, 7, -10),
								material_new(REFLECTIVE,
									  color3f_new(0.2, 0.37, 0.7),
									  color3f_new(0.2, 0.2, 0.2),
									  color3f_new(0.7, 0.7, 0.7), 2, 0.4)); 
	
	/* Light with origin somewhere top and a bit further */
	Light light = light_new(vec3_new(20, 12, 10), vec3_new(-1, -1, -2), color3f_new(0.4, 0.4, 0.4), 55.0f);
	
	objects[0] = s1;
	objects[1] = s2;
	objects[2] = p1;
	objects[3] = s3;
	objects[4] = t1;
	
	bmp16* bmp = bmp16_create_new(BMP_ID, width, height);
	bmp16_pixel pix;
	pix.r = 160;
	pix.g = 196;
	pix.b = 255;
	int i, j, s, r, k;
	Camera c;
	Ray ray;
	float max_dist = -1.0f;
	float min_distance = 1000000.0f;
	Ray rays[RAYS_PER_PIXEL];
	float light_factor = 2;
	Intersection in;
	
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			float avg_r = 0, avg_b = 0, avg_g = 0;
			for (r = 0; r < RAYS_PER_PIXEL; r++)
			{
				/* Generate ray */
				rays[r] = generate_pixel_ray(RANDOM, &camera, i, j, RAYS_PER_PIXEL, r);
				/* Trace the color */
				in = ray_trace(&rays[r], objects, n_objects, &light, 0);
				/* Accumulate for the rays */
				avg_r += in.col.r;
				avg_g += in.col.g;
				avg_b += in.col.b;
			}

			pix.r = (unsigned char)(255 * (avg_r / (RAYS_PER_PIXEL)));
			pix.g = (unsigned char)(255 * (avg_g / (RAYS_PER_PIXEL)));
			pix.b = (unsigned char)(255 * (avg_b / (RAYS_PER_PIXEL)));
			
			bmp16_set_pixel_at(bmp, pix, i, j);
		}
	}
	printf("Done looping. Max distance: %f, Min distance %f;", max_dist, min_distance);
	bmp16_save(bmp, "output/result.bmp");
	
	free(bmp->pixels);
	free(bmp);

	return 0;
}

/*
* Camera orientation and translation 
* https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
* https://steveharveynz.wordpress.com/2012/12/20/ray-tracer-part-two-creating-the-camera/
*
* j - width (Screen coord - X)
* i - height (Screen coord - Y)
* bx and by - bias from bottom left
*/
Ray pixel_trace(Camera *camera, int i, int j, float bx, float by)
{
	/* The carhtesian coordinate system. */
	M4x4 world_coordinate_space = m4x4_create_identity();
	M4x4 camera_translate = m4x4_translate(vec3_new(-1, 3, 1));
	M4x4 camera_coordinate_space = m4x4_mul(camera_translate, world_coordinate_space);
	
	/* Fake a rotation since the rotation matrix is not yet supported */
	// camera_coordinate_space.m[0][0] += 0.5;
	// camera_coordinate_space.m[1][0] += 0.5;
	// camera_coordinate_space.m[2][0] += 0.5;
	// camera_coordinate_space.m[0][1] -= 0.5;
	// camera_coordinate_space.m[1][1] += 0.5;
	// camera_coordinate_space.m[2][1] -= 0.5;
	
	Ray ray;
	ray.origin = camera->origin;
	
	/* Normalized Device Coordinates. Locating the (biased by bx/by) centers of the pixel. */
	float NDCx = (j + bx) / camera->screen.width;
	float NDCy = (i + by) / camera->screen.height;
	
	/* Screen Space. Remapping from [0,1] to [-1,1] */
	float SCx = (2*NDCx) - 1;
	float SCy = (2*NDCy) - 1;
	
	/* Calculate the half of the angle (split by the central view line) and convert it to radians */
	float halfARRad = (camera->fov / 2) * (M_PI / 180);

	/* Camera space. Aligning with field of view and aspect ratio. */
	float Cx = SCx * camera->screen.aspect_ratio * tan(halfARRad) * camera->plane_distance;
	float Cy = SCy * tan(halfARRad) * camera->plane_distance;

	/* Default camera orientation is the negative Z direction. Cx, Cy, -1 */
	/* Transform both the origin and direction to the camera's coordinate system */
	/* (Or we can transform the whole direction after substracting the origin and the point) */
	ray.origin = m4x4_mul_vec3(camera_coordinate_space, ray.origin);
	ray.direction = m4x4_mul_vec3(camera_coordinate_space, vec3_new(Cx, Cy, -1));
	ray.direction = vec3_sub(ray.direction, ray.origin);
	vec3_normalize(&ray.direction);

	return ray;
}

Ray generate_pixel_ray(SamplingMethod method, Camera* camera, int i, int j, int maxRays, int currentRay)
{
	float bx, by;
	switch(method)
	{
		case SINGLE_CENTER_RAY:
		{
			bx = 1/2;
			by = 1/2;
			break;
		}
		case DISTRIBUTED:
		{
			float dx = 1 / (float)sqrt(maxRays);
			float dy = 1 / (float)sqrt(maxRays);
			bx = ((currentRay % 4) * dx / 2);
			by = ((int)(currentRay / 4) * dy / 2);
			break;
		}
		case JITTERED:
		{
			/* NOT IMPLEMENTED */
			bx = 1/2;
			by = 1/2;
			break;
		}
		case RANDOM:
		{
			bx = 1.0f / ((rand() % RAYS_PER_PIXEL) + 1);
			by = 1.0f / ((rand() % RAYS_PER_PIXEL) + 1);
			break;
		}
		default:
		{
			bx = 1/2;
			by = 1/2;
		}
	}
	
	return pixel_trace(camera, i, j, bx, by);
}

Intersection ray_sphere_intersection(Ray *ray, Sphere *sphere)
{
	Intersection res;
	res.has_intersection = 0;
	res.distance = 0;
	
	float dist_to_center = vec3_distance(ray->origin, sphere->origin);
	Vec3 L = vec3_sub(sphere->origin, ray->origin);
	float tca = vec3_dot_product(L, ray->direction);
	if (tca < 0)
		return res;
	
	float d2 = vec3_dot_product(L, L) - tca * tca;
	float thc = sqrt(pow(sphere->radius, 2) - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;
	
	
	if (t0 > t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0) return res;
	}
	
	float coefficient = t0;
	if (d2 < sphere->radius * sphere->radius)
	{
		res.has_intersection = 1;
		res.point = vec3_add(ray->origin, vec3_mul(ray->direction, coefficient));
		res.distance = vec3_distance(ray->origin, res.point);
	}
	
	return res;
}

Intersection ray_plane_intersection(Ray *ray, Plane *plane)
{
	Intersection res;
	res.has_intersection = 0;
	res.distance = 10000;
	Vec3 intersection_point;
	Vec3 point_ray_origin = vec3_sub(ray->origin, plane->point);
	float d1 = vec3_dot_product(point_ray_origin, plane->normal);
	float d2 = vec3_dot_product(ray->direction, plane->normal) * (-1);
	float t = d1 / d2;
	
	intersection_point = vec3_add(ray->origin, vec3_mul(ray->direction, t));
	float intersects = vec3_dot_product(vec3_sub(plane->point, intersection_point), plane->normal);
	if (fabsf(intersects) <= EPSILON && t > 0)
	{
		/* printf("I %.25f, T: %.25f\n", intersects, t); */
		res.has_intersection = 1;
		res.distance = vec3_distance(ray->origin, intersection_point);
		res.point = intersection_point;
	}
	
	return res;
}

Intersection ray_triangle_intersection(Ray *ray, Triangle *triangle)
{
	Intersection res;
	res.has_intersection = 0;
	Vec3 P;
	/* 
	   Compute the tri's normal, assuming CCW winding
	   We get the two vertecies e0 and e1 represented by v1-v0 and v2-v0
	*/
	Vec3 e0 = vec3_sub(triangle->v1, triangle->v0);
	Vec3 e1 = vec3_sub(triangle->v2, triangle->v1);
	Vec3 e2 = vec3_sub(triangle->v0, triangle->v2);
	Vec3 N = vec3_cross_product(e0, e1);
	
	/* Check the angle between the ray and the triangle.
	   If its near 0, ray is parallel to the triangle. 
	*/
	float ndotray = vec3_dot_product(N, ray->direction);
	if (fabs(ndotray) < EPSILON)
		return res;
	
	/* Choose randomly one of the points on the triangle's plane, e.g. v0 */
	/* Find the point P where the ray intersects the triangle plane */
	Vec3 point_to_origin = vec3_sub(ray->origin, triangle->v0);
	float nom = vec3_dot_product(point_to_origin, N);
	float denom = vec3_dot_product(ray->direction, N) * (-1);
	float t = nom/denom;
	
	/* P = O + tD, for given t */
	P = vec3_add(ray->origin, vec3_mul(ray->direction, t));
	if (t < 0)
		return res;
	
	/* VP is the vector from the intersection point and the vertex */
	/* C is the normal vector given by the VP and the edge */
	Vec3 vp = vec3_sub(P, triangle->v0);
	Vec3 C = vec3_cross_product(e0, vp);

	if (vec3_dot_product(N, C) < 0)
		return res;
	
	vp = vec3_sub(P, triangle->v1);
	C = vec3_cross_product(e1, vp);
	
	if (vec3_dot_product(N, C) < 0)
		return res;
	
	vp = vec3_sub(P, triangle->v2);
	C = vec3_cross_product(e2, vp);
	
	if (vec3_dot_product(N, C) < 0)
		return res;
	
	if (t > 0)
	{
		res.has_intersection = 1;
		res.distance = vec3_distance(ray->origin, P);
		res.point = P;
	}
	
	return res;
}

Intersection intersects(Ray *ray, Object *objects, int count)
{
	Intersection in;
	Intersection res_in;
	/* Default color of the sky */
	res_in.col = color3f_new(0.627, 0.768, 1);
	res_in.has_intersection = 0;
	
	/* Get the intersection point of the first met object */
	int i;
	Object hit;
	float nearest_dist = 10000000;
	for (i = 0; i < count; i++)
	{
		Object c = objects[i];
		if (c.type == SPHERE)
		{
			in = ray_sphere_intersection(ray, &(c.Obj.sphere));
		}
		else if (c.type == PLANE)
		{
			in = ray_plane_intersection(ray, &(c.Obj.plane));
		}
		else if (c.type == TRIANGLE)
		{
			in = ray_triangle_intersection(ray, &(c.Obj.triangle));
		}
		
		if (in.has_intersection > 0)
		{
			if (nearest_dist >= in.distance)
			{
				res_in.has_intersection = 1;
				res_in.obj = c;
				res_in.point = in.point;
				
				nearest_dist = in.distance;
			}
		}	
	}
	
	return res_in;
}

Intersection ray_trace(Ray *ray, Object *objects, int count, Light *light, int depth)
{
	Intersection res_in = intersects(ray, objects, count);
	
	if (res_in.has_intersection)
	{
		Color3f a = ambient(ray, &res_in);
		Color3f ph = phong(ray, &res_in, light, objects, count);
		Color3f re = reflection(ray, &res_in, objects, count, light, depth);
		
		res_in.col = color3f_clamp(color3f_add(re, color3f_add(a, ph)));
		
		return res_in;
	}
	
	return res_in;
}

Color3f ambient(Ray *ray, Intersection *intersection)
{
	return color3f_mul_color(intersection->obj.mat.ambient, 0.4); /* Hardcoded scene ambient coefficient */
}

/*
  --------------------------------------------------------------------
						getDiffuseAndSpecularLighting(..)
   https://github.com/marczych/RayTracer/blob/master/src/RayTracer.cpp
  --------------------------------------------------------------------
*/
Color3f phong(Ray *ray, Intersection *intersection, Light *light, Object* objects, int count)
{
	Color3f diffuse = color3f_new(0, 0, 0);
	Color3f specular = color3f_new(0, 0, 0);
	
	Vec3 p = intersection->point;
	Vec3 n = obj_surface_normal(p, &intersection->obj);
	Vec3 s = vec3_sub(light->origin, intersection->point);
	Vec3 ld = light->direction;
	
	vec3_normalize(&n);
	vec3_normalize(&s);
	
	Ray shadow_ray;
	shadow_ray.direction = s;
	shadow_ray.origin = vec3_add_delta(&p, &n);

	if (intersects(&shadow_ray, objects, count).has_intersection)
	{
		/* Do nothing? */
	}
	else
	{
		Vec3 sr = vec3_reflect(vec3_mul(s, -1), n);
		float intens = 0, sintens = 0;
		
		intens = vec3_dot_product(s, n);
		/* Both diffuse and specular should be included only if the dotproduct is positive */
		if (intens < 0) 
			intens = 0;
		else /* The specular component should be included only if we have diffuse at the point */
		{
			sintens = vec3_dot_product(sr, vec3_mul(ray->direction, -1));
			if (sintens < 0) sintens = 0;
		}
		
		/* 
			Ka*Ia + (Kd*(L.N)*Id + Ks(R.V)*Is) 
		*/
		diffuse = color3f_mul_color(intersection->obj.mat.diffuse, intens);
		specular = color3f_mul_color(intersection->obj.mat.specular, (float)pow(sintens, intersection->obj.mat.shininess));
		Color3f phong = color3f_add(diffuse, specular);
		return phong;
	}
	
	return color3f_add(diffuse, specular);
}

Color3f reflection(Ray *ray, Intersection *intersection, Object *objects, int count, Light* light, int depth)
{
	Color3f c = color3f_new(0, 0, 0);
	
	if (depth < MAX_DEPTH && intersection->obj.mat.type == REFLECTIVE)
	{
		Vec3 sphere_normal = obj_surface_normal(intersection->point, &intersection->obj);
	
		Ray next;
		next.direction = vec3_reflect(ray->direction, sphere_normal);
		next.origin = vec3_add_delta(&intersection->point, &sphere_normal);
		
		depth += 1;
		Intersection in2 = ray_trace(&next, objects, count, light, depth);
		if (in2.has_intersection)
		{
			c = color3f_mul_color(in2.col, intersection->obj.mat.reflectivity);
		}
	}
	 
	return c;
}