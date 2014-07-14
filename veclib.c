/*
 * Author: Tyler Allen
 * Date: 03/05/13
 *
 * A library of vector functions for use in the ray tracer.
 *
 * Note: This file must be compiled with -lm.
 */

/* The header file for this library file.*/
#include "veclib.h"

/*
 * This function finds the dot product of vec1 and vec2, and returns it.
 *
 * Param: vec1  The first vector to find the product of.
 * Param: vec2  The second vector to find the product of with vec1.
 * 
 * Returns: The final dot product.
 */
double dot3 (double* vec1, double* vec2)
{
    return vec1[X] * vec2[X] + 
           vec1[Y] * vec2[Y] + 
           vec1[Z] * vec2[Z];
}

/*
 * This function scales an input vector by a scale factor, and stores it into
 * a new vector.
 *
 * Param: scale_factor  The factor by which to scale the input vector.
 * Param: input_vector  The vector to be scaled.
 * Param: output_vector The vector to store the new output vector in.
 */
void scale3 (double scale_factor, double* input_vec, double* output_vec)
{
    output_vec[X] = input_vec[X] * scale_factor;
    output_vec[Y] = input_vec[Y] * scale_factor;
    output_vec[Z] = input_vec[Z] * scale_factor;
}

/*
 * This function finds the length of an input vector.
 * 
 * Param: vec1  The vector to find the length of.
 * 
 * Return: The calculated length of vec1.
 */
double length3 (double* vec1)
{
    return sqrt(pow(vec1[X], SQUARED) + 
                pow(vec1[Y], SQUARED) + 
                pow(vec1[Z], SQUARED));
}


/* 
 * Finds the difference between two input vectors.
 * 
 * Param: v1  The vector to subtract.
 * Param: v2  The vector to subtract from.
 * Param: v3  The vector to store the new vector in.
 */
void diff3 (double* v1, double* v2, double* v3)
{
    v3[X] = v2[X] - v1[X];
    v3[Y] = v2[Y] - v1[Y];
    v3[Z] = v2[Z] - v1[Z];
}

/*
 * Adds two vectors into a new vector.
 * 
 * Param: v1  The first vector to add.
 * Param: v2  The second vector to add.
 * Param: v3  The vector to store the sum of v1 and v2 in.
 */
void sum3 (double* v1, double* v2, double* v3)
{
    v3[X] = v2[X] + v1[X];
    v3[Y] = v2[Y] + v1[Y];
    v3[Z] = v2[Z] + v1[Z];
}

/*
 * Calculates the unit vector of an input vector. This function will print to
 * stderr if a vector of [0, 0, 0] is input. 
 * 
 * Param: v1  The vector to find the unit vector of.
 * Param: v2  The vector to store the unit vector in.
 */
void unitvec3 (double* v1, double* v2)
{
    double scalar = length3(v1);
    if (scalar)
    {
        scalar = ONE/scalar;
        v2[X] = v1[X] * scalar;
        v2[Y] = v1[Y] * scalar;
        v2[Z] = v1[Z] * scalar;
    }
    else
    {
        v2[X] = ZERO;
        v2[Y] = ZERO;
        v2[Z] = ZERO;
    }
}

/*
 * unitin - unit vector in the incoming direction
 * unitnorm - outward surface normal
 * unitout - unit vector in direction of bounce
 */

void reflect3 (double* unitin, double* unitnorm, double* unitout)
{
    double U[XYZ];
    scale3(-1, unitin, U);
    double N[XYZ];
    copy3(unitnorm, N);
    unitvec3(N, N); 
    double big_N[XYZ];
    scale3(2 * dot3(U, N), N, big_N);
    diff3(U, big_N, unitout);
    unitvec3(unitout, unitout);
}

/*
 * Function for copying a vector into another vector. Realistically, it does not
 * have to be exclusively vectors, but any 3 index array of doubles.
 *
 * Param: vec1  Input vector to copy.
 * Param: vec2  Output vector to copy to.
 */
void copy3(double* vec1, double* vec2)
{
    vec2[X] = vec1[X];
    vec2[Y] = vec1[Y];
    vec2[Z] = vec1[Z];
}

/*
 * A function that prints a given input vector of any size.
 * 
 * Param: label   A pointer to the label to print before the vector. Used for 
 *                human readability in order to keep track of what vector is 
 *                being printed.
 * Param: vector  The vector to be printed.
 * Param: vector_size  The size of the vector that is being printed.
 */
void vecprnN (char* label, double* vector, int vector_size)
{
    int i;
    fprintf (stderr, "%s\n", label);
    for (i = ZERO; i < vector_size; i++)
    {
        fprintf (stderr, "   %f", vector[i]);
    }    
    fprintf(stderr, "\n");
}
