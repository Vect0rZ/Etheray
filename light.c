#include "light.h"

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