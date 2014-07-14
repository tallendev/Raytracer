/*
 * Author: Tyler Allen
 * Date: 03/15/2013
 *
 * This is the header file for the Light object.
 */

/* Ensures this header file can only be included once. */
#pragma once

/* The correct number of objects to be read in during sphere_init. */
#define LIGHT_OBJS 6

/* Includes the Malloc function and is_light function. */
#include "utils.h"

/* Includes object structs and necessary definitions. */
#include "object.h"

/*
 * The light_type struct for use in private data in object structs.
 * Contains the location of the light in the world.
 *
 * Emissivity: The strength of the light emitted by this object.
 * Data Member: location  The location of the light source.
 * Data Member: priv  An optional pointer to a more specific kind of light,
 *                    such as a spotlight.
 * Function Member: getemiss Function pointer used to return the emissivity
 *                           of this object.
 */
typedef struct light_type
{
    double emissivity[RGB_SIZE];
    double location[DIMENSIONS];
    void* priv;
    void    (*getemiss)(struct light_type*, double *);
    int     (*illum_check)(obj_t* lobj, double* hitloc);
} light_t;

obj_t* light_init(FILE* in, int objtype);

void default_getemiss(light_t* light, double* value);

void dump_light(FILE* out, obj_t* obj);

void kill_light(void* light);
