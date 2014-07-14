/*
 * Author: Tyler Allen
 * Date: 3/15/2013
 *
 * Header file for the projection.c file.
 */

/* Ensures this header file is only included once. */
#pragma once

/* Includes several #defines, and the object type.*/
#include "object.h"

/* Includes utility functions such as Malloc for use in projection.c*/
#include "utils.h"

#define X_BY_Y 2

/*
 * Struct containing the size of the screen in pixel size, the size of the
 * world in world values, and the location of the viewpoint. This structure
 * has been typdefined as proj_t.
 *
 * Data Member: win_size_pixel The window size in pixels.
 * Data Member: win_size_world The window size in world coordinates.
 * Data Member: view_point The point at which the viewer resides.
 */
typedef struct projection_type 
{
    int win_size_pixel[X_BY_Y];
    double win_size_world[X_BY_Y];
    double view_point[DIMENSIONS];
} proj_t;

proj_t* projection_init(int x, int y, FILE* in);

void projection_dump(FILE* out, proj_t* proj);

void map_pix_to_world(proj_t* proj, int x, int y, double* world);
