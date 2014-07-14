/*
 * Author: Tyler Allen
 * Date: 03/14/2013
 *
 * This file contains the information for initializing our projection and also
 * dumping it after we are done reading it in.
 */

/* 
 * Header file for this source file. Contains necessary includes 
 * and structures.
 */
#include "projection.h"

/*
 * This file creates an initial projection. It will return a proj_t pointer
 * that has been allocated memory and had values set.
 *
 * Param: x  The size of the window, in pixels, in the x direction. 
 *           Received from the command line.
 * Param: y  The size of the window, in pixels, in the y direction.
 *           Received from the command line.
 * Return: proj  The fully initialized projection (type proj_t pointer).
 */
proj_t* projection_init(int x, int y, FILE* in)
{
    char buff[BUFF_SIZE];
    proj_t* proj = Malloc(sizeof(proj_t));
    proj->win_size_pixel[X] = x;
    proj->win_size_pixel[Y] = y;
    fscanf(in, " %lf %lf",
            &proj->win_size_world[X],
            &proj->win_size_world[Y]);
    fgets(buff, BUFF_SIZE, in);
    fscanf(in, " %lf %lf %lf",
            &proj->view_point[X],
            &proj->view_point[Y],
            &proj->view_point[Z]);
    fgets(buff, BUFF_SIZE, in);
    return proj;     
}

/*
 * The dump function for projections. This is for debugging purposes, to ensure
 * that information that has been read in is properly handled. It will print 
 * all the information in the projection.
 *
 * Param: out  The stream to output to.
 * Param: proj The projection of who's contents are to be dumped.
 */
void projection_dump(FILE* out, proj_t* proj)
{
    fprintf(out, "Dumping Projection Data\nWindow Size in Pixels:\n"
                 "X: %d\nY: %d\n\nWindow Size in World Coordinates:\n"
                 "X: %f\nY: %f\n\n View Point:\nX: %f\nY: %f\nZ: %f\n"
                 "End of Projection Dump\n",
                 proj->win_size_pixel[X],
                 proj->win_size_pixel[Y],
                 proj->win_size_world[X], 
                 proj-> win_size_world[Y],
                 proj->view_point[X],
                 proj->view_point[Y],
                 proj->view_point[Z]);
}
