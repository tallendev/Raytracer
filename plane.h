/*
 * Author: Tyler Allen
 * Date: 03/14/2013
 *
 * The header file for the plane source file.
 */

/* Ensures this header file is not included multiple times. */
#pragma once

/* The proper number of objects to read in during plane_init. */
#define PLANE_OBJS 6

/* An approximation of zero that allows us to account for minor floating point
 * rounding errors that may occur when calculating a hit. */
#define ROUNDING_ADJUSTMENT 1e-14 //000

/* Includes standard libraries and the Malloc function. */
#include "utils.h"

#include "veclib.h"

/* Includes the object structs and other necessary structs. */
#include "object.h"

/* 
 * The plane_type struct child struct for obj_t structs. Contains information
 * necessary for constructing a plane. These are all infinite planes.
 *
 * Data Member: point  Some point on the plane.
 * Data Member: normal The normal (perpendicular) vector of the plane, 
 *              determining the orientation.
 */
typedef struct plane_type
{
    double point[VEC_SIZE];
    double normal[VEC_SIZE];
    void *priv;
} plane_t;

obj_t* plane_init(FILE* in, int objtype);

void dump_plane(FILE* out, obj_t* obj);

double hits_plane(double* base, double* dir, obj_t* obj);

void kill_plane(void* plane);
