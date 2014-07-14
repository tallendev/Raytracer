/*
 * Author: Tyler Allen
 * Date: 03/08/2013
 *
 * The header file for the utils.c file. It includes the necessary #includes 
 * and function prototypes.
 */

/* Allow this file to only be included once. */
#pragma once

/* Included primarily for use of printf functions and reading in input. */
#include <stdio.h>

/* Include for the use of the malloc and calloc functions. */
#include <stdlib.h>

/* Our enumeration of our objects. */
#define FIRST_TYPE   10
#define LIGHT        10
#define SPOTLIGHT    11
#define PROJECTOR    12
#define SPHERE       13
#define PLANE        14
#define FINITE_PLANE 15
#define TILED_PLANE  16
#define TEX_PLANE    17
#define REF_SPHERE   18
#define P_SPHERE     19
#define P_PLANE      20
#define PARABOLOID   21
#define CYLINDER     22
#define CONE         23
#define HYPERBOLOID  24
#define LAST_TYPE    25
#define LAST_LIGHT   12

#define BUFF_SIZE 256

/* Used as boolean values. */
#define TRUE 1
#define FALSE 0

void* Malloc(size_t size);

void* Calloc(int spaces, size_t size);

int is_light(int objtype);

int vec_get1(FILE* in, double* ndx);
