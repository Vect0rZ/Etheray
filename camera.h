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

Camera camera_create(Vec3 origin, float fov, float pDist, int w, int h);

#endif