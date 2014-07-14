/* 
 * @Author: Tyler Allen
 * @Date: 04/01/2013
 *
 * This is the header file for image.c. It contains function prototypes and 
 * necessary includes.
 */

/* Ensures we only include this header file once. */
#pragma once

/* Includes the wrapper malloc function. */
#include "utils.h"

/* Includes the raytracing functions that this file uses to create its 
 * images. */
#include "raytrace.h"

/* Included for anti-aliasing. */
#include <time.h>

/* Number of AA samples if defined */
#ifdef AA
    #define AA_SAMPLES 8
#endif
#ifndef AA
    #define AA_SAMPLES 1
#endif

void map_pix_to_world(proj_t* proj, int x, int y, double* world);

void make_pixel(model_t *model, int x, int y, unsigned char *pixval);

void make_image(model_t* model);

double randpix(double x);
