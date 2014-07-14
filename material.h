/*
 * Author: Tyler Allen
 * Date: 04/01/2013
 *
 * This file is the header file for the material.c source file. It contains 
 * the function prototype for material_init, in addition to the material 
 * struct typedef.
 */

#pragma once
#define RGB_SIZE 3

/* Forward declarion of the material_t type. */
typedef struct material_type material_t;

/*
 * The structure that contains material data. This is usually a part of 
 * non-light obj_t's.
 *
 * Data Member: ambient  The RGB array containing our information about ambient
 *                       light.
 * Data Member: diffuse  The RGB array containing our information about diffuse
 *                       light.
 * Data Member: specular The RGB array containing our information about specular
 *                       light.
 */
struct material_type
{
    double ambient [RGB_SIZE];
    double diffuse [RGB_SIZE];
    double specular [RGB_SIZE];
};

/* Included for the obj_t typedef for material_load. */
#include "object.h"

int material_load(FILE* in, material_t* material);

void default_getamb(obj_t* obj, double* output);

void default_getdiff(obj_t* obj, double* output);

void default_getspec(obj_t* obj, double* output);

void print_materials(FILE* out, material_t material);
