#ifndef CAMERA_H
#define CAMERA_H

#include "vector3.h"

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

/**
 * Creates a new camera by given 
 * location, field of view, view plane distance, width and height
 * */
Camera camera_new(Vec3 origin, float fov, float pDist, int w, int h);

#endif
