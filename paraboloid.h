/*
 * Author: Tyler Allen
 * Date: 04/21/2013
 *
 * This is the header file for the parabaloid source file.
 */

/* Ensures this file can only be included once. */
#pragma once

/* Included for the obj_t structure. */
#include "object.h"
/* Included for the Malloc function and stdlib. */
#include "utils.h"
/* Included for doing matrix operations. */
#include "matlib.h"
/* Defines the number of dimensions in XYZ. */
#define XYZ 3
/* Constant for a squaring operation. */
#define SQUARED 2
/* Defining the correct number of objects to read in from the input file. */
#define PARAB_OBJS 8

/*
 * This type contains the informtation necessary to simulate
 * a paraboloid. 
 *
 * Data Member: center Center point of this paraboloid.
 * Data Member: centerline A line through the center of the paraboloid.
 * Data Member: radius The radius of this paraboloid.
 * Data Member: height The height of this paraboloid.
 * Data Member: scale (r^2)/h
 * Data Member: rotmat  The rotation matrix necessary for rotating this object
 *              to make hit detection easier.
 * Data Member: irot  The transpose of rotmat, for reversing the transformation.
 */
typedef struct parab_type
{
    double center[XYZ];
    double centerline[XYZ];
    double radius;
    double height;
    double scale; /* r^2 / h */
    double rotmat[XYZ][XYZ];
    double irot[XYZ][XYZ];
} parab_t;

obj_t* parab_init(FILE* in, int objtype);

double parab_hits(double* base, double* dir, obj_t* obj);

void dump_parab (FILE* out, obj_t* obj);

void kill_parab(void* parab);

double check_parab_hit(obj_t* obj, double* dir, double* newbase, double t);
