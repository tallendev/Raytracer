/*
 * Author: Tyler Allen
 * Date: 04/11/2013
 *
 * This is the header file for the fplane.c source file.
 */

/* Ensures this file is only included once. */
#pragma once

/* Included for the obj_t typedef. */
#include "object.h"
/* Included for the plane object type. */
#include "plane.h"
/* Included for matrix functions. */
#include "matlib.h"

#define XYZ 3
#define XY 2
#define FPLANE_OBJS 5

/*
 * This contains the data necessary for a finite plane to be built.
 *
 * Data Member: xdir  The xdirection of this plane.
 * Data Member: size  The length/width of this plane.
 * Data Member: rotmat  The rotation matrix that we use to move this plane to 
 *                      make simplified calculations.
 * Data Member: lasthit  Used for textures.
 * Data Member: priv  Used for plugging in expansions on the fplane.
 */
typedef struct fplane_type
{
    double xdir[XYZ];
    double size[XY];
    double rotmat[XYZ][XYZ];
    double lasthit[XY];
    void* priv;
} fplane_t;

obj_t* fplane_init(FILE* in, int objtype);

double hits_fplane(double* base, double* dir, obj_t* obj);

void dump_fplane(FILE* out, obj_t* obj);

void kill_fplane(void* priv);
