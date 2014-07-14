/*
 * Author: Tyler Allen
 * Date: 03/15/2013
 *
 * The header file for the sphere.c source file. 
 */

/* Ensures this header file is only included once. */
#pragma once

/* Includes the Malloc function. */
#include "utils.h"

#include "veclib.h"

/* Contains the object_type struct. */
#include "object.h"

/* The proper number of objects for sphere to read in.*/
#define SPHERE_OBJS 4

/* 
 * This struct contains the sphere type struct, for storing sphere data in
 * the priv data of objects. 
 *
 * Data Member: center The point at the center of the sphere.
 * Data Member: radius The radius of the sphere.
 */
typedef struct sphere
{
    double center[DIMENSIONS];
    double radius;
} sphere_t;

obj_t* sphere_init(FILE* in, int objtype);

double hits_sphere(double* base, double* dir, obj_t* obj);

void dump_sphere(FILE* out, obj_t* obj);

void kill_sphere(void* sphere);
