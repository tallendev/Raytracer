/*
 * Author: Tyler Allen
 * Date: 04/19/2013
 *
 * This is the header file for the spotlight source file.
 */

/* Ensures this file is only included once. */
#pragma once

/* Includes the light type. */
#include "light.h"
/* Includes the Malloc function. */
#include "utils.h"
/* Definition of the number of dimensions are tracer models in. */
#define XYZ 3
/* The correct number of objects to read in from the file. */
#define SPOTLIGHT_OBJS 4
/* 180 degrees, or half a circle. */
#define HALF_CIRCLE 180
/* 
 * Struct defining a spotlight object.
 * 
 * Data Member: direction  Cone centerline vector.
 * Data Member: theta      Half-width of cone in deg
 * Data Member: costheta   precomputed cos(theta)
 */
typedef struct spotlight_type
{
    double direction[XYZ];
    double theta;
    double costheta;
} spotlight_t;

obj_t* spotlight_init(FILE* in, int objtype);

void dump_spotlight(FILE* out, obj_t* obj);

int spotlight_check(obj_t* lobj, double* hitloc);

void kill_spotlight(void* spotlight);
