/*
 * Author: Tyler Allen
 * Date: 03/13/2013
 *
 * This source file contains functions that operate on sphere objects. Sphere
 * objects are obj_ts that have sphere_t's in the private data of the object.
 */

/* The header file for this source file. */
#include "sphere.h"

/*
 * Initializes an obj_t struct, and then a sphere_t struct to store inside the
 * private data of the previous obj_t. 
 *
 * Param: in  The stream to read in from.
 * Param: objtype  The type of the object being created.
 *
 * Return: obj  The object that has been constructed and initialized.
 */
obj_t* sphere_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    obj_t* obj    = NULL;
    int pcount    = 0;
    obj = object_init(in, objtype);
    sphere_t* sphere = Malloc(sizeof(sphere_t));
    pcount += fscanf(in, "%lf %lf %lf", 
                     &sphere->center[X],
                    &sphere->center[Y],
                    &sphere->center[Z]);
    fgets(buff, BUFF_SIZE, in);
    pcount += fscanf(in, "%lf", &sphere->radius);
    fgets(buff, BUFF_SIZE, in);
    if (sphere->radius < 0 || pcount != SPHERE_OBJS || obj == NULL)
    {
        fprintf(stderr, "Error found in sphere_init...");
        obj = NULL;
    }
    else
    {
        obj->priv = (void*)sphere;
        obj->hits = hits_sphere;//
    }
    return obj;
}

/*
 * Function for determining if a ray hit a sphere.
 *
 * Param: base  The starting point for the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The sphere object that is being tested for a hit.
 *
 * Return: t_sub_h The calculated distance to the object, or -1 if we miss.
 */
double hits_sphere(double *base, double *dir, obj_t *obj)
{ 
    obj->hitloc[X] = FP_NAN;
    obj->hitloc[Y] = FP_NAN;
    obj->hitloc[Z] = FP_NAN;
    obj->normal[X] = FP_NAN;
    obj->normal[Y] = FP_NAN;
    obj->normal[Z] = FP_NAN;
    double t_sub_h = MISS;
    sphere_t* sphere = (sphere_t*)obj->priv;
    double* d = Malloc(sizeof(double) * DIMENSIONS);
    unitvec3(dir, d);
    double a = dot3(d, d);
    double* vprime = Malloc(sizeof(double) * DIMENSIONS);
    diff3(sphere->center, base, vprime);
    /* Here we have a few magic numbers for the quadratic formula.*/
    double b = 2 * dot3(vprime, d);
    double c = dot3(vprime, vprime) - sphere->radius * sphere->radius;
    double discrim = (b * b) - (4 * a * c);
    free(vprime);
    if(discrim >= 0)
    {
        double* scaled_d = Malloc(sizeof(double) * DIMENSIONS);
        t_sub_h = ((-1 * b) - sqrt(discrim))/((2 * a));
        scale3(t_sub_h, d, scaled_d);
        sum3(base, scaled_d, obj->hitloc);
        
        double* hitloc_center = Malloc(sizeof(double) * DIMENSIONS);
        diff3(sphere->center, obj->hitloc, hitloc_center);
        unitvec3(hitloc_center, obj->normal);
        
        free(hitloc_center);
        free(scaled_d);
    }
    free(d);
    return t_sub_h;
}

/*
 * Function for dumping the data of a sphere.
 * Param: out  The output stream to dump to.
 * Param: obj  The object to dump.
 */
void dump_sphere(FILE* out, obj_t* obj)
{
    fprintf(out, "\nSphere Data\n");
    sphere_t* sphere = obj->priv;
    fprintf(out, "\nCenter:  %f  %f  %f\nRadius: %f\n\n", 
            sphere->center[X], sphere->center[Y],
            sphere->center[Z], sphere->radius);
}

/*
 * Function to allow a sphere to kill itself.
 *
 * Param: sphere  The private data of an obj_t of type sphere.
 */
void kill_sphere(void* sphere) 
{
    free(sphere);
}
