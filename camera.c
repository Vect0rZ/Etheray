#include <stdio.h>
#include "camera.h"

Camera camera_create(Vec3 origin, float fov, float pDist, int w, int h)
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