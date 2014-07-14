/*
 * Author: Tyler Allen
 * Date: 03/13/2013
 *
 * This source file contains the functions for initializing planes and 
 * determining if a ray hits a plane.
 */

/* The header file for this source file. */
#include "plane.h"

/* 
 * This function initalizes an obj_t's priv data field with plane data, in 
 * addition to calling the obj_t constructor function.
 * 
 * Param: in  The file to read in from.
 * Param: objtype The object type to be assigned to the object.
 *
 * Return: obj  The constructed and initialized object.
 */
obj_t* plane_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    obj_t* obj = NULL;
    int pcount = 0;
    obj = object_init(in, objtype);
    plane_t* plane = Malloc(sizeof(plane_t));
    pcount += fscanf(in, "%lf %lf %lf",
                     &plane->normal[X],
                     &plane->normal[Y],
                     &plane->normal[Z]);
    fgets(buff, BUFF_SIZE, in);
    pcount += fscanf(in, "%lf %lf %lf",
                     &plane->point[X],
                     &plane->point[Y],
                     &plane->point[Z]);
    fgets(buff, BUFF_SIZE, in);
    if (pcount != PLANE_OBJS || obj == NULL)
    {
        fprintf(stderr, "Error found in plane_init...\n");
        obj = NULL;
    }
    else
    {
        obj->hits = hits_plane;
        obj->priv = plane;
    }
    return obj;
}

/*
 * Function for determining if a ray hit a plane.
 *
 * Param: base  The starting point for the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The plane object that is being tested for hits.
 *
 * Return: The distance from our viewpoint that the object is at, if we hit.
 *         If we miss, returns -1.
 */
double hits_plane(double* base, double* dir, obj_t* obj)
{ 
    obj->hitloc[X] = FP_NAN; /* Start values at NAN to help debug. */
    obj->hitloc[Y] = FP_NAN;
    obj->hitloc[Z] = FP_NAN;
    obj->normal[X] = FP_NAN;
    obj->normal[Y] = FP_NAN;
    obj->normal[Z] = FP_NAN;
    double* d = Malloc(sizeof(double) * DIMENSIONS);
    unitvec3(dir, d);
    plane_t* plane = (plane_t*)obj->priv;
    double n_dot_d = dot3(plane->normal, d);
    double n_dot_q = dot3(plane->normal, plane->point);
    if (0 == n_dot_d)
    {
        free(d);
        return MISS;
    }
    double n_dot_v = dot3(plane->normal, base);
    double t_sub_h = (n_dot_q - n_dot_v) / n_dot_d;
    if (0 > t_sub_h)
    {
        free(d);
        return MISS;
    }
    double* scaled_d = Malloc(sizeof(double) * DIMENSIONS);
    scale3(t_sub_h, d, scaled_d);
    sum3(base, scaled_d, obj->hitloc);
    free(scaled_d);
    free(d);
    if (obj->hitloc[Z] > ROUNDING_ADJUSTMENT)
    {
        return MISS;
    }
    else
    {
        obj->normal[X] = plane->normal[X];
        obj->normal[Y] = plane->normal[Y];
        obj->normal[Z] = plane->normal[Z];
    }
    return t_sub_h;
}

/*
 * Function for dumping the data about plane objects.
 * Param: out  The stream to dump to.
 * Param: obj  The object to dump.
 */
void dump_plane(FILE* out, obj_t* obj)
{
    fprintf(out, "\nPlane Data\n");
    plane_t* plane = obj->priv;
    fprintf(out, "\nPoint: %f %f %f\nNormal:  %f"
            "  %f  %f\n", 
            plane->point[X], plane->point[Y], 
            plane->point[Z], plane->normal[X], 
            plane->normal[Y], plane->normal[Z]);

}

/*
 * Function allowing a plane to kill itself and free its memory.
 * Param: plane  Plane to kill.
 */
void kill_plane(void* plane)
{
    free(plane);
}
