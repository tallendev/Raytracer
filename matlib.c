/*
 * Author: Tyler Allen
 * Date: 04/11/2013
 *
 * This source file contains common matrix operations to be used for 
 * calculations in finite objects.
 */

/* Header for this source file. */
#include "matlib.h"

/* 
 * Square 3x3 matrix multiplication function. This allows the output matrix
 * to be one of the two input functions. 
 *
 * Param: mat1  The left matrix to multiply.
 * Param: mat2  The right matrix to multiply.
 * Param: mat3  The output matrix.
 */
void matrix_mult(double mat1[XYZ][XYZ], double mat2[XYZ][XYZ],
                 double mat3[XYZ][XYZ])
{
    double temp[XYZ][XYZ];
    for (int i = 0; i < XYZ; i++)
    {
        for (int j = 0; j < XYZ; j++)
        {
            temp[i][j] = 0;
            for (int k = 0; k < XYZ; k++)
            {
                temp[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    for (int i = 0; i < XYZ; i++)
    {
        for (int j = 0; j < XYZ; j++)
        {
            mat3[i][j] = temp[i][j];
        }
    }
}

/* 
 * Multiplication between a matrix and a vector. The output vector may be 
 * the same as the input vector. 
 *
 * Param: mat1  The matrix to multiply
 * Param: vec1  The vector to multiply
 * Parm: output The output vector
 */
void matrix_vec(double mat1[XYZ][XYZ], double vec1[XYZ], double output[XYZ])
{
    double temp[XYZ];
    for (int i = 0; i < XYZ; i++)
    {
        temp[i] = dot3(mat1[i], vec1);
    }
    for (int i = 0; i < XYZ; i++)
    {
        output[i] = temp[i];
    }
}

/*
 * This is a function to transpose a matrix.
 * Param: mat1  The matrix to transpose.
 */
void transpose_mat(double mat1[XYZ][XYZ], double mat2[XYZ][XYZ])
{
    for (int i = 0; i < XYZ; i++)
    {
        mat2[i][i] = mat1[i][i];
    }
    for (int i = 1; i < XYZ; i++)
    {
        for (int j = 0; j < i; j++)
        {
            double temp = mat1[i][j];
            mat2[i][j] = mat1[j][i];
            mat2[j][i] = temp;
        }
    }
}

/* 
 * This function finds the cross product of the two input matrices.
 *
 * Param: V The first matrix to multiply.
 * Param: W The second matrix to multiply.
 * Param: output  The output matrix to store the cross product in.
 */
void cross_prod(double V[XYZ], double W[XYZ], double output[XYZ])
{
    double temp[XYZ];
    temp[X] = (V[Y] * W[Z] - V[Z] * W[Y]);
    temp[Y] = (V[Z] * W[X] - V[X] * W[Z]);
    temp[Z] = (V[X] * W[Y] - V[Y] * W[X]);
    for (int i = 0; i < XYZ; i++)
    {
        output[i] = temp[i];
    }
}

/*
 * This function finds the projection of V onto N and stores it in P.
 * REQUIRES UNIT VECTORS.
 * Param: V  The vector to be projected onto N.  
 * Param: N The vector to that will have V project onto it.
 * Param: P The output vector for this process.
 */
void projection(double V[XYZ], double N[XYZ], double P[XYZ])
{
    double scaled_n[XYZ];
    scale3(dot3(V, N), N, scaled_n);
    diff3(scaled_n, V, P);
}

/*
 * Returns a copy of the identity matrix statically defined within this 
 * function.
 * Param: The matrix to output into.
 */
void get_id_matrix(double output[XYZ][XYZ])
{
    static double id_matrix[XYZ][XYZ] = 
    {
        {1, 0, 0}, 
        {0, 1, 0}, 
        {0, 0, 1}
    };
    copy_matrix(id_matrix, output);
}

/*
 * Performs a transformation on the input vector using the rotation matrix, and 
 * returns the output vector.
 * Param: mat1  The rotation matrix.
 * Param: vec1  The vector to transform with.
 * Param: vec2  The output vector.
 */
void transform(double mat1[XYZ][XYZ], double vec1[XYZ], double vec2[XYZ])
{
    double temp[XYZ];
    for (int i = 0; i < XYZ; i++)
    {
        temp[i] = 0;
        for (int j = 0; j < XYZ; j++)
        {
            temp[i] += mat1[i][j] * vec1[j];
        }
    }
    for(int i = 0; i < XYZ; i++)
    {
        vec2[i] = temp[i];
    }
}

/*
 * Prints an input matrix with a leading label describing the output.
 * Param: label  The label to print representing the matrix.
 * Param: mat    The matrix to print.
 */
void matrix_print(char* label, double mat[XYZ][XYZ])
{
    fprintf(stderr, "%s\n", label);
    for (int i = 0; i < XYZ; i++)
    {
        for (int j = 0; j < XYZ; j++)
        {
            fprintf(stderr, "%f ", mat[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

/*
 * Copies a matrix into another matrix.
 * Param: mat1  The matrix to be copied.
 * Param: mat2  The matrix to copy to.
 */
void copy_matrix(double mat1[XYZ][XYZ], double mat2[XYZ][XYZ])
{
    for (int i = 0; i < XYZ; i++)
    {
        for(int j = 0; j < XYZ; j++)
        {
            mat2[i][j] = mat1[i][j];
        }
    }
}
