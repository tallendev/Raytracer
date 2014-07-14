/*
 * Author: Tyler Allen
 * Date: 03/15/2013
 *
 * The header file for the source file model.c. It contains necessary includes
 * and defines.
 */

/* Ensures this file is only included once. */
#pragma once

/* These are all also necessary for access to their dump functions. */
/* Necessary for the light_t structure. */
#include "light.h"
/* Necessary for the projection_t structure. */
#include "projection.h"
/* Necessary for the list_t structure. */
#include "linked_list.h"
/* Necessary for the sphere_t structure. */
#include "sphere.h"
/* Necessary for the plane_t structure. */
#include "plane.h"
/* Necessary for pplane_t structure. */
#include "pplane.h"
/* Necessary for psphere_t structure. */
#include "psphere.h"
/* Necessary for fplane_t structure. */
#include "fplane.h"
/* Necessary for tplane_t structure. */
#include "tplane.h"
/* Necessary for spotlight_t data. */
#include "spotlight.h"
/* Necessary for cyl_t data structure. */
#include "cylinder.h"
/* Necessary for parab_t data structure. */
#include "paraboloid.h"
/* Necessary for cone_t data structure. */
#include "cone.h"
/* Necessary for hyperb_t data structure and functions. */
#include "hyperboloid.h"

/* 
 * Structure of a model, representing the image to be drawn. 
 * 
 * Data Member: proj  The projection that stores world information, such as 
 *                    viewpoint, world size, and screen size in pixels.
 * Data Member: lights  A linked list of all lights contained in this model.
 * Data Member: scene   A linked list of all scene objects in this model.
 */
typedef struct model_type
{
    proj_t* proj;
    list_t* lights;
    list_t* scene;
} model_t;

int model_init(FILE* in, model_t* model);

void model_dump(FILE* out, model_t* model);

void dump_object(FILE* out, obj_t* obj);

obj_t* dummy_init(FILE* in, int objtype);

obj_t* create_objects(int* objtype, obj_t* obj, int* rc, FILE* in);

void dump_dummy(FILE* out, obj_t* obj);

