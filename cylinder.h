/*
 * Author: Tyler Allen
 * Date: 04/20/2013
 *
 * Header file for the cylinder.c source file.
 */

/* Ensures this header file is only included once. */
#pragma once

/* Includes stdlib, and the Malloc function. */
#include "utils.h"

/* Includes the obj_t structure and init function. */
#include "object.h"

/* Includes the matrix math library file that we have created. */
#include "matlib.h"

/* The number of dimensions that we are modeling in. */
#define XYZ 3

/* The correct number of objects to read in from the input file. */
#define CYL_OBJS 8

/*
 * This type contains the information necessary to simulate
 * a cylinder.
 *
 * Data Member: center Center point of this cylinder.
 * Data Member: centerline A line through the center of the cylinder.
 * Data Member: radius The radius of this cylinder.
 * Data Member: height The height of this cylinder.
 * Data Member: rotmat  The rotation matrix necessary for rotating this object
 *              to make hit detection easier.
 * Data Member: irot  The transpose of rotmat, for reversing the transformation.
 */
typedef struct cyl_type
{
    double center[XYZ];
    double centerline[XYZ];
    double radius;
    double height;
    double rotmat[XYZ][XYZ];
    double irot[XYZ][XYZ];
} cyl_t;

obj_t* cyl_init(FILE* in, int objtype);

double cyl_hits(double* base, double* dir, obj_t* obj);

void dump_cyl(FILE* out, obj_t* obj);

void kill_cyl(void* cylinder);

double check_cyl_hit(obj_t* obj, double* dir, double* newbase, double t);
