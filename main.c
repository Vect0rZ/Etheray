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

typedef struct screen
{
	int width;
	int height;
	float aspect_ratio;
}Screen;

typedef struct camera
{
	Vec3 origin;
	float fov;
	float plane_distance;
	Screen screen;
}Camera;

typedef struct ray
{
	Vec3 origin;
	Vec3 direction;
} Ray;

typedef struct light
{
	Vec3 origin;
	Vec3 direction;
	float intensity;
	Color3f ambient;
}Light;


typedef struct intersection_data
{
	int has_intersection;
	float distance;
	Vec3 point;
	Object obj;
	Color3f col;
}Intersection;

typedef struct header
{
	short id;
	int size;
	short non1;
	short non2;
	int pixel_array_offset;
}bmp_header;

typedef struct pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}bmp16_pixel;

typedef struct dib
{
	int width;
	int height;
	short col_planes;
	short bits;
	int bi_rgb;
	int raw_size;
	int dpi_horizontal;
	int dpi_vertical;
	int col_palette;
	int col_important;
}bmp16_dib;

/*
	The pixel array should be a seperate structure
	so the get_pixel and set_pixel work independently
	from the bmp structure.
*/
typedef struct bmp
{
	bmp_header header;
	int dib_size;
	bmp16_dib dib;
	bmp16_pixel* pixels;
	int pixel_count;
	int pad;
} bmp16;

long get_file_size(FILE* fd);
/* Begin New methods */
bmp16* read_bmp_single_read(const char* file);
short get_short(unsigned char** data);
int get_int(unsigned char** data);
bmp16_pixel get_pixel(unsigned char** data);
void skip_pad(unsigned char** data, int pad);
void save_optimized(bmp16* bmp);
bmp16* create_new(int id, int width, int height);
void set_data(unsigned char** arr, unsigned char* data, int size);
/* Process helper methods */
unsigned char truncate(int color);
bmp16_pixel get_at(bmp16* bmp, int i, int j);
void set_at(bmp16* bmp, bmp16_pixel pixel, int i, int j);
/* End New methods */

/* Matrix methods */

/* Create methods */


Camera create_camera(Vec3 origin, float fov, float pDist, int w, int h);
/* Raytracing methods */
Ray pixel_trace(Camera *camera, int i, int j, float bx, float by);
Ray generate_pixel_ray(SamplingMethod method, Camera* camera, int i, int j, int maxRays, int currentRay);
Ray random_pixel_ray(Camera *camera, int i, int j);
Intersection ray_sphere_intersection(Ray *ray, Sphere *sphere);
Intersection ray_plane_intersection(Ray *ray, Plane *plane);
Intersection ray_triangle_intersection(Ray *ray, Triangle *triangle);
Intersection intersects(Ray *ray, Object *objects, int count);
Intersection ray_trace(Ray *ray, Object *objects, int count, Light *light, int depth);
Ray reflection_ray(Vec3 point, Vec3 normal);
/* Light methods */
Light create_light(Vec3 origin, Vec3 direction, Color3f ambient, float intensity);
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
	
	Camera camera = create_camera(create_vec3(0, 0, 0), 70, 1, width, height);
	Object s1 = create_sphere(create_vec3(2, 1, -15), 2,
							  c_mat(REFLECTIVE,
									c_col3f(0.811, 0.988, 0.454), 
									c_col3f(0.36, 0.36, 0.36), 
									c_col3f(0.7, 0.7, 0.7),
									20, 0.7));
	Object s2 = create_sphere(create_vec3(-6, 0, -15), 4,
							  c_mat(REFLECTIVE,
								c_col3f(0.282, 0.376, 0.498), 
								c_col3f(0.7, 0.7, 0.7), 
								c_col3f(0.2, 0.2, 0.2),
								10, 0.07));
	Object s3 = create_sphere(create_vec3(2.5, 2, -7), 1.2, 
							  c_mat(REFLECTIVE,
									c_col3f(1, 0.223, 0.301), 
									c_col3f(0.1, 0.1, 0.1), 
									c_col3f(0.36, 0.36, 0.36),
									20, 0.4));
	/* Plane located on the origin, and with normal pointing UP. (XZ plane) */
	Object p1 = create_plane(create_vec3(0, -4, 0), create_vec3(0, 1, 0),
							 c_mat(REFLECTIVE,
								   c_col3f(0.1, 0.1, 0.1), 
								   c_col3f(0.1, 0.1, 0.1), 
								   c_col3f(0.1, 0.1, 0.1),
								   7, 0.2));
	 Object t1 = create_triangle(create_vec3(-7, 4, -15), create_vec3(7, 4, -15), create_vec3(0, 7, -10),
								c_mat(REFLECTIVE,
									  c_col3f(0.2, 0.37, 0.7),
									  c_col3f(0.2, 0.2, 0.2),
									  c_col3f(0.7, 0.7, 0.7), 2, 0.4)); 
	
	/* Light with origin somewhere top and a bit further */
	Light light = create_light(create_vec3(20, 12, 10), create_vec3(-1, -1, -2), c_col3f(0.4, 0.4, 0.4), 55.0f);
	
	objects[0] = s1;
	objects[1] = s2;
	objects[2] = p1;
	objects[3] = s3;
	objects[4] = t1;
	
	bmp16* bmp = create_new(BMP_ID, width, height);
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
			
			set_at(bmp, pix, i, j);
		}
	}
	printf("Done looping. Max distance: %f, Min distance %f;", max_dist, min_distance);
	save_optimized(bmp);
	free(bmp->pixels);
	free(bmp);
    return 0;
}

long get_file_size(FILE* fd)
{
	long size = 0;
	fseek(fd, 0, SEEK_END);
	size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	
	return size;
}

int debug = 0;
bmp16* read_bmp_single_read(const char* file)
{
	bmp16* bmp = malloc(sizeof(bmp16));
	FILE* fd = fopen(file, "rb+");
	
	long fsize = get_file_size(fd);
	unsigned char* data = malloc(sizeof(unsigned char) * fsize);
	int fres = fread(data, sizeof(unsigned char), fsize, fd);
	fclose(fd);
	
	if (fres != fsize)
		return 0;

	bmp->header.id = get_short(&data);
	bmp->header.size = get_int(&data);
	bmp->header.non1 = get_short(&data);
	bmp->header.non2 = get_short(&data);
	bmp->header.pixel_array_offset = get_int(&data);
	
	bmp->dib_size = get_int(&data);
	bmp->dib.width = get_int(&data);
	bmp->dib.height = get_int(&data);
	bmp->dib.col_planes = get_short(&data);
	bmp->dib.bits = get_short(&data);
	bmp->dib.bi_rgb = get_int(&data);
	bmp->dib.raw_size = get_int(&data);
	bmp->dib.dpi_horizontal = get_int(&data);
	bmp->dib.dpi_vertical = get_int(&data);
	bmp->dib.col_palette = get_int(&data);
	bmp->dib.col_important = get_int(&data);

	int i, j, ind = 0;
	int width = bmp->dib.width;
	int height = bmp->dib.height;
	bmp->pixel_count = width * height;
	bmp->pad = 0;
	int row_bytes = width * BYTES_PER_PIXEL_24;
	if ((row_bytes) % 4 != 0)
		bmp->pad = 4 - (row_bytes % 4);
	printf("1.Debug\n");
	printf("PAD: %i\n", bmp->pad);
	bmp->pixels = malloc(sizeof(bmp16_pixel) * bmp->pixel_count);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			bmp->pixels[ind++] = get_pixel(&data);
		}
		skip_pad(&data, bmp->pad);
	}

	return bmp;
}

short get_short(unsigned char** data)
{
	short res = *((short*)*data);
	*data += sizeof(short);
	debug += sizeof(short);
	return res;
}

int get_int(unsigned char** data)
{
	int res = *((int*)*data);
	*data += sizeof(int);
	debug += sizeof(int);
	return res;
}

bmp16_pixel get_pixel(unsigned char** data)
{
	bmp16_pixel pixel;
	char components[3];
	components[0] = *((*data)++);
	components[1] = *((*data)++);
	components[2] = *((*data)++);
	int col = *((int*)components);

	pixel.b = (col & 0x00FF0000) >> 16; debug++;
	pixel.g = (col & 0x0000FF00) >> 8;debug++;
	pixel.r = (col & 0x000000FF);debug++;
	
	return pixel;
}

void skip_pad(unsigned char** data, int pad)
{
	(*data) += pad;
}

/*
	Reads an int from a given file descriptor and advances the stream by sizeof(int) bytes
*/
int read_int(FILE* fd)
{
	int result;
	fread(&result, sizeof(int), 1, fd);
	
	return result;
}

void save_optimized(bmp16* bmp)
{
	FILE* fd = fopen("output/result.bmp", "wb+");
	int file_size = sizeof(char) * 54 + sizeof(char)*((bmp->dib.width * bmp->dib.height * 3) + bmp->dib.width * bmp->pad);
	unsigned char* file = malloc(file_size);
	unsigned char* file_cpy = file;
	unsigned char pad[2] = {0,0};
	int debug = 0;
	bmp16_pixel* pix_cpy = bmp->pixels;
	
	set_data(&file_cpy, (unsigned char*)&bmp->header.id, sizeof(short));
	set_data(&file_cpy, (unsigned char*)&bmp->header.size, sizeof(int));
	set_data(&file_cpy, (unsigned char*)&bmp->header.non1, sizeof(short));
	set_data(&file_cpy, (unsigned char*)&bmp->header.non2, sizeof(short));
	set_data(&file_cpy, (unsigned char*)&bmp->header.pixel_array_offset, sizeof(int));
	set_data(&file_cpy, (unsigned char*)&bmp->dib_size, sizeof(int));
	set_data(&file_cpy, (unsigned char*)&bmp->dib, sizeof(bmp16_dib));
	
	int width = bmp->dib.width;
	int height = bmp->dib.height;
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			bmp16_pixel pixel = *pix_cpy++;
			int i_pix = 0;

			i_pix |= pixel.r << 16;
			i_pix |= pixel.g << 8;
			i_pix |= pixel.b;

			set_data(&file_cpy, (unsigned char*)&i_pix, 3);
		}

		if (bmp->pad > 0)
			set_data(&file_cpy, pad, sizeof(unsigned char) * bmp->pad);
	}
	
	fwrite(file, sizeof(unsigned char), file_size, fd);
	perror("Err");
	fclose(fd);
}

bmp16* create_new(int id, int width, int height)
{
	bmp16* bmp = malloc(sizeof(bmp16));

	bmp->header.id = id;
	bmp->header.size = 14;
	bmp->header.non1 = 0;
	bmp->header.non2 = 0;
	bmp->header.pixel_array_offset = 54;
	
	bmp->dib_size = 40;
	bmp->dib.width = width;
	bmp->dib.height = height;
	bmp->dib.col_planes = 1;
	bmp->dib.bits = 24;
	bmp->dib.bi_rgb = 0;
	bmp->dib.raw_size = width * height * BYTES_PER_PIXEL_24;
	bmp->dib.dpi_horizontal = (int)(72 * 39.3701);
	bmp->dib.dpi_vertical = (int)(72 * 39.3701);
	bmp->dib.col_palette = 0;
	bmp->dib.col_important = 0;

	bmp->pixel_count = width * height;
	bmp->pad = 0;
	int row_bytes = width * BYTES_PER_PIXEL_24;
	if ((row_bytes) % 4 != 0)
		bmp->pad = 4 - (row_bytes % 4);

	bmp->pixels = malloc(sizeof(bmp16_pixel) * bmp->pixel_count);
	
	return bmp;
}

void set_data(unsigned char** arr, unsigned char* data, int size)
{
	memcpy(*arr, data, size);
	(*arr) += size;
	/* while (size > 0)
	   {
	  	 *(*arr) = *data++;
	  	 (*arr)++;
	  	 size--;
	   }
	*/
}

unsigned char truncate(int color)
{
	if (color < 0)
		return 0;
	if (color > 255)
		return 255;
	
	return color;
}

bmp16_pixel get_at(bmp16* bmp, int i, int j)
{
	int index = (i * bmp->dib.width) + j;
	
	return bmp->pixels[index];
}

void set_at(bmp16* bmp, bmp16_pixel pixel, int i, int j)
{
	int index = (i * bmp->dib.width) + j;
	bmp->pixels[index] = pixel;
}

Camera create_camera(Vec3 origin, float fov, float pDist, int w, int h)
{
	Camera res;
	Screen sc;
	
	res.origin = origin;
	res.fov = fov;
	res.plane_distance = pDist;
	
	sc.width = w;
	sc.height = h;
	sc.aspect_ratio = (float)w / (float)h;
	printf("W: %i, H: %i\n", w, h);
	
	
	res.screen = sc;
	
	printf("AR: %f", sc.aspect_ratio);
	return res;
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
	M4x4 world_coordinate_space = create_identity_m4x4();
	M4x4 camera_translate = translate_m4x4(create_vec3(-1, 3, 1));
	M4x4 camera_coordinate_space = mul_m4x4(camera_translate, world_coordinate_space);
	
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
	ray.origin = mul_m4x4_vec3(camera_coordinate_space, ray.origin);
	ray.direction = mul_m4x4_vec3(camera_coordinate_space, create_vec3(Cx, Cy, -1));
	ray.direction = sub_vectors(ray.direction, ray.origin);
	normalize(&ray.direction);

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

Ray random_pixel_ray(Camera *camera, int i, int j)
{
	float bx = 1.0f / ((rand() % RAYS_PER_PIXEL) + 1);
	float by = 1.0f / ((rand() % RAYS_PER_PIXEL) + 1);

	return pixel_trace(camera, i, j, bx, by);
}

Intersection ray_sphere_intersection(Ray *ray, Sphere *sphere)
{
	Intersection res;
	res.has_intersection = 0;
	res.distance = 0;
	
	float dist_to_center = dist(ray->origin, sphere->origin);
	Vec3 L = sub_vectors(sphere->origin, ray->origin);
	float tca = dot_product(L, ray->direction);
	if (tca < 0)
		return res;
	
	float d2 = dot_product(L, L) - tca * tca;
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
		res.point = add_vectors(ray->origin, mul_vector(ray->direction, coefficient));
		res.distance = dist(ray->origin, res.point);
	}
	
	return res;
}

Intersection ray_plane_intersection(Ray *ray, Plane *plane)
{
	Intersection res;
	res.has_intersection = 0;
	res.distance = 10000;
	Vec3 intersection_point;
	Vec3 point_ray_origin = sub_vectors(ray->origin, plane->point);
	float d1 = dot_product(point_ray_origin, plane->normal);
	float d2 = dot_product(ray->direction, plane->normal) * (-1);
	float t = d1 / d2;
	
	intersection_point = add_vectors(ray->origin, mul_vector(ray->direction, t));
	float intersects = dot_product(sub_vectors(plane->point, intersection_point), plane->normal);
	if (fabsf(intersects) <= EPSILON && t > 0)
	{
		/* printf("I %.25f, T: %.25f\n", intersects, t); */
		res.has_intersection = 1;
		res.distance = dist(ray->origin, intersection_point);
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
	Vec3 e0 = sub_vectors(triangle->v1, triangle->v0);
	Vec3 e1 = sub_vectors(triangle->v2, triangle->v1);
	Vec3 e2 = sub_vectors(triangle->v0, triangle->v2);
	Vec3 N = cross_product(e0, e1);
	
	/* Check the angle between the ray and the triangle.
	   If its near 0, ray is parallel to the triangle. 
	*/
	float ndotray = dot_product(N, ray->direction);
	if (fabs(ndotray) < EPSILON)
		return res;
	
	/* Choose randomly one of the points on the triangle's plane, e.g. v0 */
	/* Find the point P where the ray intersects the triangle plane */
	Vec3 point_to_origin = sub_vectors(ray->origin, triangle->v0);
	float nom = dot_product(point_to_origin, N);
	float denom = dot_product(ray->direction, N) * (-1);
	float t = nom/denom;
	
	/* P = O + tD, for given t */
	P = add_vectors(ray->origin, mul_vector(ray->direction, t));
	if (t < 0)
		return res;
	
	/* VP is the vector from the intersection point and the vertex */
	/* C is the normal vector given by the VP and the edge */
	Vec3 vp = sub_vectors(P, triangle->v0);
	Vec3 C = cross_product(e0, vp);

	if (dot_product(N, C) < 0)
		return res;
	
	vp = sub_vectors(P, triangle->v1);
	C = cross_product(e1, vp);
	
	if (dot_product(N, C) < 0)
		return res;
	
	vp = sub_vectors(P, triangle->v2);
	C = cross_product(e2, vp);
	
	if (dot_product(N, C) < 0)
		return res;
	
	if (t > 0)
	{
		res.has_intersection = 1;
		res.distance = dist(ray->origin, P);
		res.point = P;
	}
	
	return res;
}

Intersection intersects(Ray *ray, Object *objects, int count)
{
	Intersection in;
	Intersection res_in;
	/* Default color of the sky */
	res_in.col = c_col3f(0.627, 0.768, 1);
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
		
		res_in.col = cl_c3f(add_c3f(re, add_c3f(a, ph)));
		
		return res_in;
	}
	
	return res_in;
}

Vec3 reflect(Vec3 in, Vec3 surface_normal)
{
	Vec3 refl;
	float dot = dot_product(in, surface_normal);
	Vec3 double_normal = mul_vector(mul_vector(surface_normal, 2), dot);
	refl = sub_vectors(in, double_normal);
	normalize(&refl);
	return refl;
}

Light create_light(Vec3 origin, Vec3 direction, Color3f ambient, float intensity)
{
	Light res;
	res.origin = origin;
	res.intensity = intensity;
	res.direction = direction;
	res.ambient = ambient;
	
	normalize(&res.direction);
	
	return res;
}

Color3f ambient(Ray *ray, Intersection *intersection)
{
	return mul_c3f_c(intersection->obj.mat.ambient, 0.4); /* Hardcoded scene ambient coefficient */
}

/*
  --------------------------------------------------------------------
						getDiffuseAndSpecularLighting(..)
   https://github.com/marczych/RayTracer/blob/master/src/RayTracer.cpp
  --------------------------------------------------------------------
*/
Color3f phong(Ray *ray, Intersection *intersection, Light *light, Object* objects, int count)
{
	Color3f diffuse = c_col3f(0, 0, 0);
	Color3f specular = c_col3f(0, 0, 0);
	
	Vec3 p = intersection->point;
	Vec3 n = surface_normal(p, &intersection->obj);
	Vec3 s = sub_vectors(light->origin, intersection->point);
	Vec3 ld = light->direction;
	
	normalize(&n);
	normalize(&s);
	
	Ray shadow_ray;
	shadow_ray.direction = s;
	shadow_ray.origin = fix_point(&p, &n);

	if (intersects(&shadow_ray, objects, count).has_intersection)
	{
		/* Do nothing? */
	}
	else
	{
		Vec3 sr = reflect(mul_vector(s, -1), n);
		float intens = 0, sintens = 0;
		
		intens = dot_product(s, n);
		/* Both diffuse and specular should be included only if the dotproduct is positive */
		if (intens < 0) 
			intens = 0;
		else /* The specular component should be included only if we have diffuse at the point */
		{
			sintens = dot_product(sr, mul_vector(ray->direction, -1));
			if (sintens < 0) sintens = 0;
		}
		
		/* 
			Ka*Ia + (Kd*(L.N)*Id + Ks(R.V)*Is) 
		*/
		diffuse = mul_c3f_c(intersection->obj.mat.diffuse, intens);
		specular = mul_c3f_c(intersection->obj.mat.specular, (float)pow(sintens, intersection->obj.mat.shininess));
		Color3f phong = add_c3f(diffuse, specular);
		return phong;
	}
	
	return add_c3f(diffuse, specular);
}

Color3f reflection(Ray *ray, Intersection *intersection, Object *objects, int count, Light* light, int depth)
{
	Color3f c = c_col3f(0, 0, 0);
	
	if (depth < MAX_DEPTH && intersection->obj.mat.type == REFLECTIVE)
	{
		Vec3 sphere_normal = surface_normal(intersection->point, &intersection->obj);
	
		Ray next;
		next.direction = reflect(ray->direction, sphere_normal);
		next.origin = fix_point(&intersection->point, &sphere_normal);
		
		depth += 1;
		Intersection in2 = ray_trace(&next, objects, count, light, depth);
		if (in2.has_intersection)
		{
			c = mul_c3f_c(in2.col, intersection->obj.mat.reflectivity);
		}
	}
	 
	return c;
}