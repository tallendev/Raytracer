/*
 * Author: Tyler Allen
 * Date: 04/06/2013
 *
 * This file is the header file for the procedural sphere (psphere.c) 
 * source file.
 *
 */

/* Ensure that we only include this header file once.*/
#pragma once

/* Includes the obj_t typedef. */
#include "object.h"
/* Includes the sphere_init function that we need. */
#include "sphere.h"
/* Includes the readin functions and the FILE type. */
#include "stdio.h"
/* Included for using math functions in the procedural formulas. */
#include <math.h>
/* Includes rand and some other functions. */
#include <stdlib.h>
/* Includes time in case we rand. */
#include <time.h>
/* Includes vec_get1 function. */
#include "utils.h"

obj_t* psphere_init(FILE* in, int objtype);

void psphere0_amb(obj_t* obj, double* value); 

void psphere1_amb(obj_t* obj, double* value);

void psphere2_amb(obj_t* obj, double* value);

void psphere3_amb(obj_t* obj, double* value);
