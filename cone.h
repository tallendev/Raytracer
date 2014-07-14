/*
 * Author: Tyler Allen
 * Date: 04/29/2013
 *
 * This is the header file for the cone.c source file.
 */

/* Ensures this header file is only included once. */
#pragma once

/* Included for the ob_t struct.*/
#include "object.h"
/* Included for stdio, and the Malloc function. */
#include "utils.h"
/* Included for matrix functions. */
#include "matlib.h"

/* The number of dimensions in our world. */
#define XYZ 3

/* The correct number of items to read in for a cone. */
#define CONE_OBJS 8 

/* Power of 2. */
#define SQUARED 2

/*
 * This type contains the information necessary to simulate
 * a paraboloid. 
 *
 * Data Member: center Center point of this paraboloid.
 * Data Member: centerline A line through the center of the paraboloid.
 * Data Member: radius The radius of this paraboloid.
 * Data Member: height The height of this paraboloid.
 * Data Member: scale  The magnitude by which to scale the shape based on its
 *                     height and radius.
 * Data Member: rotmat  The rotation matrix necessary for rotating this object
 *              to make hit detection easier.
 * Data Member: irot  The transpose of rotmat, for reversing the transformation.
 */
typedef struct cone_type
{
    double center[XYZ];
    double centerline[XYZ];
    double radius;
    double height;
    double scale; /* r^2 / h */
    double rotmat[XYZ][XYZ];
    double irot[XYZ][XYZ];
} cone_t;

obj_t* cone_init(FILE* in, int objtype);

double cone_hits(double* base, double* dir_start, obj_t* obj);

double check_cone_hit(obj_t* obj, double* dir, double* newbase, double t);

void dump_cone(FILE* out, obj_t* obj);

void kill_cone(void* cone);
