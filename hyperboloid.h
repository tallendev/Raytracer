/*
 * Author: Tyler Allen
 * Date: 05/02/2013
 *
 * Header file for the hyperboloid.c source file.
 */

/* Ensures this header file is only included once. */
#pragma once

/* Necessary for the obj_t struct. */
#include "object.h"

/* The correct number of items to read in for this object.*/
#define HYPER_OBJS 9

/*
 * This type contains the information necessary to simulate
 * a hyperboloid. 
 *
 * Data Member: center Center point of this hyperboloid.
 * Data Member: centerline A line through the center of the hyperboloid.
 * Data Member: radius The radius of this hyperboloid.
 * Data Member: height The height of this hyperboloid.
 * Data Member: scale  The magnitude by which to scale the shape based on its
 *                     height and radius.
 * Data Member: rotmat  The rotation matrix necessary for rotating this object
 *              to make hit detection easier.
 * Data Member: irot  The transpose of rotmat, for reversing the transformation.
 */
typedef struct hyperb_type
{
    double center[XYZ];
    double centerline[XYZ];
    double radius;
    double radiusc;
    double height;
    double scale;
    double rotmat[XYZ][XYZ];
    double irot[XYZ][XYZ];
} hyperb_t;

obj_t* hyperb_init(FILE* in, int objtype);

double hyperb_hits(double* base, double* dir_start, obj_t* obj);

double check_hyperb_hit(obj_t* obj, double* dir, double* newbase, double t);

void dump_hyperb(FILE* out, obj_t* obj);

void kill_hyperb(void* hyperb);
