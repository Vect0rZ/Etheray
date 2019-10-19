#include "material.h"

Material material_create(MaterialType type, Color3f ambient, Color3f specular, Color3f diffuse, float shininess, float reflectivity)
{
	Material res;
	res.type = type;
	res.ambient = ambient;
	res.specular = specular;
	res.diffuse = diffuse;
	res.shininess = shininess;
	res.reflectivity = reflectivity;
	
	return res;
}