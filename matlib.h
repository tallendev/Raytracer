/*
 * Author: Tyler Allen
 * Date: 04/11/2013
 *
 * This is the header file for the matlib.c source file
 */

/* Ensures this source file is only included once. */
#pragma once

/* Included for the dot3 function. */
#include "veclib.h"

#define X 0
#define Y 1
#define Z 2
#define XYZ 3

void matrix_mult(double mat1[XYZ][XYZ], double mat2[XYZ][XYZ], 
                 double mat3[XYZ][XYZ]);

void matrix_vec(double mat1[XYZ][XYZ], double vec1[XYZ], double output[XYZ]);

void transpose_mat(double mat1[XYZ][XYZ], double mat2[XYZ][XYZ]);

void cross_prod(double V[XYZ], double W[XYZ], double output[XYZ]);

void projection(double V[XYZ], double N[XYZ], double P[XYZ]);

void get_id_matrix(double output[XYZ][XYZ]);

void matrix_print(char* label, double mat[XYZ][XYZ]);

void copy_matrix(double mat1[XYZ][XYZ], double mat2[XYZ][XYZ]);

void transform(double mat1[XYZ][XYZ], double vec1[XYZ], double vec2[XYZ]);
