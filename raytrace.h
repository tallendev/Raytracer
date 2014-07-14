/*
 * @Author: Tyler Allen
 * @Date: 04/01/2013
 *
 * This file contains the function prototypes and includes used in the raytrace
 * source file.
 */
#pragma once

/* Number defining our cap on reflective bounces. */
#define MAX_DIST 70.0 //0.0 //70.0 //30.0

/* This contains the model_t struct that we use in our functions.*/
#include "model.h"
/* This includes various vector functions used in determining if a ray hits 
 * an object. */
#include "veclib.h"
/* Includes the functions for calculating diffuse lighting. */
#include "illuminate.h"

void ray_trace(model_t* model, double base[DIMENSIONS], double dir[DIMENSIONS], 
               double intensity[DIMENSIONS], double total_dist, 
               obj_t* last_hit);

obj_t* find_closest_object(list_t* scene, double base[DIMENSIONS], 
                            double dir[DIMENSIONS], 
                            obj_t* last_hit, double* mindist);
