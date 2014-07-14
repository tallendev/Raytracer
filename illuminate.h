/*
 * Author: Tyler Allen
 * Date: 04/09/2013
 *
 * This is the header file to the source file illuminate.c
 */

/* Ensures that this header file is included only once. */
#pragma once

/* Included for the obj_t typedef. */
#include "object.h"
/* Included so that we can iterate through lists. */
#include "linked_list.h"
/* Included so that we can access the model object in diffuse_illumination() */
#include "model.h"
/* Used so that we can calculate unit vecs and differences. */
#include "veclib.h"
/* Included so that we may call the closest_object function. */
#include "raytrace.h"

void diffuse_illumination(model_t* model, obj_t* hitobj, double* intensity);

int process_light (list_t* lst, obj_t* hitobj, obj_t* lobj, double* ivec);
