#include "light.h"

Light light_new(Vec3 origin, Vec3 direction, Color3f ambient, float intensity)
{
	Light res;
	res.origin = origin;
	res.intensity = intensity;
	res.direction = direction;
	res.ambient = ambient;
	
	vec3_normalize(&res.direction);
	
	return res;
}
