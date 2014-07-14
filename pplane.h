/*
 * Author: Tyler Allen
 * Date: 04/06/2013
 *
 * This file is the header file for the procedural plane (pplane.c) source file.
 *
 */

/* Ensure that we only include this header file once.*/
#pragma once

/* Includes the obj_t typedef. */
#include "object.h"
/* Includes the plane_init function that we need. */
#include "plane.h"
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

void pplane0_amb(obj_t* obj, double* value);

void pplane1_amb(obj_t* obj, double* value);

void pplane2_amb(obj_t* obj, double* value);

void pplane3_amb(obj_t* obj, double* value);

obj_t* pplane_init(FILE* in, int objtype);
