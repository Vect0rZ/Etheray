#ifndef MATERIAL_H
#define MATERIAL_H

#include "enums.h"
#include "color3f.h"

typedef struct material
{
	MaterialType type;
	Color3f ambient;
	Color3f specular;
	Color3f diffuse;
	float shininess;
	float reflectivity;
}Material;

/**
 * Creates a new material by given
 * material type, ambient, specular and diffuse emit colors, 
 * shininess and reflectivity of the material.
 * */
Material material_new(MaterialType type, Color3f ambient, Color3f specular, Color3f diffuse, float shininess, float reflectivity);

#endif