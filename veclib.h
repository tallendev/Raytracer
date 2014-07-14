/*
 * Author: Tyler Allen
 * Date: 03/06/13
 *
 * The header file for the veclib.c file. It contains necessary #includes and 
 * function prototypes.
 */

/* Includes the Malloc and Calloc wrapper functions, as well as stdio and 
 * stdlib.
 */
/* Ensures this header file can only be included once. */
#pragma once

/* Includes the standard libraries needed. */
#include "utils.h"

/* Included for standard math functions (sqrt and pow). */
#include <math.h>

/* Index of the x dimension. */
#define X 0

/* Index of the y dimension. */
#define Y 1

/* Index of the z dimension. */
#define Z 2

/* Power for use in difference formula. */
#define SQUARED 2

/* Zero, used usually when a vector of zero is sent in to prevent divide by
 * zero errors. 
 */
#define ZERO 0

/* One, used to place over the unit vector function. */
#define ONE 1

/* Number of dimensions in XYZ, 3 */
#define XYZ 3

double dot3 (double* vec1, double* vec2);

void scale3 (double scale_factor, double* input_vec, double* output_vec);

double length3 (double* vector);

void diff3 (double* v1, double* v2, double* v3);

void sum3 (double* v1, double* v2, double* v3);

void unitvec3 (double* v1, double* v2);

void reflect3 (double* unitin, double* unitnorm, double* unitout);

void copy3(double* v1, double* v2);

void vecprnN (char* label, double* vector, int vector_size);
