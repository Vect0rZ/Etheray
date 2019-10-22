#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * BMP specific constants
 * */
#define BMP_HEADER 14
#define BYTES_PER_PIXEL_24 3
#define BMP_ID 19778

/**
 * How many rays per pixel?
 * */
#define RAYS_PER_PIXEL 1

/**
 * Maximum depth of reflection?
 * */
#define MAX_DEPTH 3

/**
 * Used to "cast back" a vector to prevent missing a surface.
 * */
#define EPSILON 0.000001

/**
 * Should debug messages be displayed?
 * */
#define DEBUG 1

#endif