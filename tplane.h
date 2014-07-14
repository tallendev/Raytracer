/*
 * Author: Tyler Allen
 * Version: 04/12/2013
 */

#pragma once

/* Included because we build off of an fplane. */
#include "fplane.h"
/* Included because we build off of a normal plane as well. */
#include "plane.h"
/* Included because all objects are obj_t type. */
#include "object.h"
/* This is used for the Malloc wrapper function. */
#include "utils.h"
/* Included because we use a material in the tplane_t struct. */
#include "material.h"

/* 
 * Struct containing the information on building a tiled plane. 
 *
 * Data Member: background  Contains the intensity of the light for every other
 *                          tile.
 */
typedef struct
{
    material_t background;
} tplane_t;

obj_t* tplane_init(FILE* in, int objtype);

void kill_tplane(void* tplane);

double hits_tplane(double* base, double* dir, obj_t* obj);

void kill_tplane(void* tplane);

void tp_spec(obj_t* obj, double* value);

void tp_amb(obj_t* obj, double* value);

void tp_diff(obj_t* obj, double* value);

int tp_select(obj_t* obj);

void dump_tplane(FILE* out, obj_t* obj);
