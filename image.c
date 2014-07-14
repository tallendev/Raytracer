/*
 * Author: Tyler Allen
 * Date: 3/15/2013
 *
 * File containing functions used for obtaining or creating the data necessary
 * to create the final image, and then write the image.
 */

/*  The header file for this source file.*/
#include "image.h"

/*
 * This function maps pixel coordinates to world coordinates.
 *
 * Param: proj   The projection containing the world size and viewpoint.
 * Param: x      The x dimension of the pix coordinate to translate.
 * Param: y      The y dimension of the pix coordinate to translate.
 * Param: world  The array to store the returned coordinates in.
 */

void map_pix_to_world(proj_t* proj, int x, int y, double* world)
{
    double rx = (double)x;
    double ry = (double)y;
    #ifdef AA
        rx = randpix(x);
        ry = randpix(y);
    #endif

    /* Transforms x pixel coordinate into x world coordinate. */
    *(world + X) = (double)(rx) / (proj->win_size_pixel[X] - 1) * 
                    proj->win_size_world[X];
    *(world + X) -= proj->win_size_world[X] / 2.0;
    /* Transforms y pixel coordinate into y world coordinate. */
    *(world + Y) = (double)(ry) / (proj->win_size_pixel[Y] - 1) * 
                    proj->win_size_world[Y];
    *(world + Y) -= proj->win_size_world[Y] / 2.0;
    /* We assume z is 0 for simplicity. */
    *(world + Z) = 0.0;
}

double randpix(double x)
{
    return x + (((double)random()/RAND_MAX) * 1.5) - .5;
}

/*
 * This function creates a pixel from the information returned by a call to
 * ray_trace. We calculate the value of "intensity", the intensity of 
 * the red, green, and blue values for each pixel.
 *
 * Param: model  The model containing necessary projection information.
 * Param: x      The x dimension of the pixel we are creating.
 * Param: y      The y dimension of the pixel we are creating.
 * Param: pixval The output pixel for this function.
 */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval)
{
    srand((unsigned)time(0));
    double world[DIMENSIONS];
    double intensity[RGB_SIZE];
    /* Finds world coordinates of pixel. */
    intensity[R] = 0;
    intensity[G] = 0;
    intensity[B] = 0;
    double* dir = Malloc(sizeof(double) * DIMENSIONS);
    for (int i = 0; i < AA_SAMPLES; i++)
    {
        map_pix_to_world(model->proj, x, y, world);
        diff3(model->proj->view_point, world, dir);
        unitvec3(dir, dir);
        /* Finds the closest object that we hit.*/
        ray_trace(model, model->proj->view_point, dir, intensity, 0.0, NULL);
    }
    free(dir);
    //fprintf(stderr, "R: %f G: %f B: %f\n", intensity[R], intensity[G], intensity[B]);
    //scale3(1/AA_SAMPLES, intensity, intensity);
    #ifdef DBG_WORLD
        fprintf(stderr, "WRL (%5.11f, %5.11f) - ", world[X], world[Y]);
    #endif
    
    /* Calculates a unit vector representing the direction from the viewer to
     * the pixel. */
    /* Clamps values over 1 back down to 1 in order to stay under 255 colors. */
    intensity[R] = intensity[R] > 1 ? 1 : intensity[R];
    intensity[G] = intensity[G] > 1 ? 1 : intensity[G];
    intensity[B] = intensity[B] > 1 ? 1 : intensity[B];
    /* Clamps values under 0 back to 0 in order to stay under 255 colors. */
    intensity[R] = intensity[R] < 0 ? 0 : intensity[R];
    intensity[G] = intensity[G] < 0 ? 0 : intensity[G];
    intensity[B] = intensity[B] < 0 ? 0 : intensity[B];
    /* Calculates the RGB colors and places them back into the return array. */
    pixval[R] = (unsigned char)(intensity[R] * MAX_COLORS);
    pixval[G] = (unsigned char)(intensity[G] * MAX_COLORS); 
    pixval[B] = (unsigned char)(intensity[B] * MAX_COLORS); 
}

/*
 * The function used for actually creating (through function calls) 
 * and writing the image. We build the image pixel by pixel, starting at the
 * bottom left, where our screen begins.
 *
 * Param: The model of which are we are attempting to draw.
 */
void make_image(model_t* model)
{
    unsigned char* pixmap = NULL;
    /* Calculates size of pixmap */
    int ppm_size = (int)(sizeof(unsigned char) * RGB_SIZE);
    ppm_size *=    (int)model->proj->win_size_pixel[X];
    ppm_size *=    (int)model->proj->win_size_pixel[Y];
    pixmap = Malloc(sizeof(unsigned char) * (size_t)ppm_size);
    for (int y = 0; y < model->proj->win_size_pixel[Y]; y++)
    {
        /* Sets memory address to the far bottom left first, to represent the
         * pixel. We work our way across the row, and then the next iteration
         * we hop back up a row and repeat, to build from bottom left. */
        int counter = (model->proj->win_size_pixel[Y] - (y + 1)) *
                  (model->proj->win_size_pixel[X] * RGB_SIZE );
        for (int x = 0; x < model->proj->win_size_pixel[X]; x++)
        {
            #ifdef DBG_PIX
                fprintf(stderr, "\nPIX %4d %4d - ", x, y);
            #endif
            make_pixel(model, x, y, &pixmap[counter]);
            counter = counter + RGB_SIZE;
         }
    }
    /* Open our output file. */
    /* Create, write, and free out PPM header info. */
    char* header = Calloc(BUFF_SIZE, sizeof(char));
    fprintf(stdout, "P6 %d %d %d\n",  model->proj->win_size_pixel[X],
                                   model->proj->win_size_pixel[Y],
                                   MAX_COLORS);
    free(header);
    #ifdef DBG_BYTES
        for(int i = 0; i < ppm_size; i++)
        {
            fprintf(stderr, "byte:%d\n", pixmap[i]);
        }
    #endif
    /* Write our pixelmap to the file, and free. */
    fwrite(pixmap, sizeof(unsigned char), (size_t)ppm_size, stdout);
    free(pixmap);
}
