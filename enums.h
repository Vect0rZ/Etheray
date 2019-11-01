#ifndef ENUMS_H
#define ENUMS_H

typedef enum sampling_method
{
	SINGLE_CENTER_RAY,
	RANDOM,
	JITTERED,
	DISTRIBUTED
}SamplingMethod;

typedef enum object_type
{
	SPHERE,
	PLANE,
	TRIANGLE
}Type;

typedef enum material_type
{
	DEFAULT,
	REFLECTIVE,
	REFRACTIVE
}MaterialType;

#endif
