/*
 * Author: Tyler Allen
 * Date: 04/29/2013
 *
 * This file contains logic for creating, destroying, and detecting hits on a
 * cone.
 */

/* Header file for this source file. */
#include "cone.h"

/*
 * Function for initializing a cone.
 * Param: in  The file we are reading in from.
 * Param: objtype  The object type of the object we are creating.
 *
 * Return:  The new object that has been created.
 */
obj_t* cone_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    int pcount = 0;
    obj_t* obj = object_init(in, objtype);
    if (obj)
    {
        cone_t* cone = Malloc(sizeof(cone_t));
        pcount += fscanf(in, "%lf %lf %lf",
                         &cone->center[X], &cone->center[Y], &cone->center[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf %lf",
                         &cone->centerline[X], &cone->centerline[Y], 
                         &cone->centerline[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount+= fscanf(in, "%lf %lf",
                        &cone->radius, &cone->height);
        fgets(buff, BUFF_SIZE, in);
        if (pcount == CONE_OBJS)
        {
            double id_matrix[XYZ][XYZ];
            get_id_matrix(id_matrix);
            /* Adjustment for shapes aligned with the negative y axis. This
             * is a minor hack to adjust for this bad case. */
            if ((cone->centerline[X] == id_matrix[Y][X] && 
                 cone->centerline[Y] < 0 &&
                 cone->centerline[Z] == id_matrix[Y][Z]))
            {
                cone->centerline[X] = ROUNDING_ADJUSTMENT;
            }
            unitvec3(cone->centerline, cone->centerline);
            copy3(cone->centerline, cone->rotmat[Y]);
            cross_prod(cone->centerline, id_matrix[Y], cone->rotmat[X]);
            unitvec3(cone->rotmat[X], cone->rotmat[X]);
            cross_prod(cone->rotmat[X], cone->rotmat[Y], cone->rotmat[Z]);
            unitvec3(cone->rotmat[Z], cone->rotmat[Z]);
            transpose_mat(cone->rotmat, cone->irot);
            cone->scale = pow(cone->radius, SQUARED) / cone->height;
            obj->hits = cone_hits;
            obj->priv = cone;
        }
        else
        {
            fprintf(stderr, "Error found in cone_init...\n");
            obj = NULL;
        }
    }
    return obj;
}

/*
 * Function for determining if a ray hit a cone.
 *
 * Param: base  The starting point for the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The plane object that is being tested for hits.
 *
 * Return: The distance from our viewpoint that the object is at, if we hit.
 *         If we miss, returns -1.
 */

double cone_hits(double* base, double* dir_start, obj_t* obj)
{
    double t = MISS;
    cone_t* cone = (cone_t*)obj->priv;
    double newbase[XYZ];
    double newcenter[XYZ];
    diff3(cone->center, base, newbase);
    double dir[XYZ];
    diff3(cone->center, cone->center, newcenter);
    double idmat[XYZ][XYZ];
    get_id_matrix(idmat);
    if (!(cone->centerline[X] == idmat[Y][X] && cone->centerline[Y] != 0 &&
        cone->centerline[Z] == idmat[Y][Z]))
    {
        transform(cone->rotmat, newbase, newbase);
        transform(cone->rotmat, dir_start, dir);
    }
    else
    {
        transform(idmat, newbase, newbase);
        transform(idmat, dir_start, dir);
    }
    double a = pow(dir[X], SQUARED) + pow(dir[Z], SQUARED) - cone->scale * 
               pow(dir[Y], SQUARED);
    double b = 2 * (newbase[X] * dir[X] + 
                   (newbase[Z] * dir[Z] - cone->scale * dir[Y] * newbase[Y]));
    double c = pow(newbase[X], SQUARED) + 
               pow(newbase[Z], SQUARED) - cone->scale * 
               pow(newbase[Y], SQUARED);
    double discrim = pow(b, SQUARED) - 4 * a * c;
    double t1 = ((-1 * b) - sqrt(discrim))/(2 * a);
    double t2 = ((-1 * b) + sqrt(discrim))/(2 * a);
    t2 = (check_cone_hit (obj, dir, newbase, t2));
    t1 = (check_cone_hit (obj, dir, newbase, t1));
    if (t1 == MISS)
    {
        t = t2;
    }
    else if (t2 == MISS)
    {
        t = t1;
    }
    else
    {
        t = t1 < t2 ? t1: t2;
    }
    return t;
}

/*
 * Function for checking both the positive and negative side of the quadratic
 * hit function.
 *
 * Param: obj  The object to check hits with and store a hitpoint in.
 * Param: dir  The direction of the current ray.
 * Param: newbase  The base of the ray we are shooting.
 * Param: t  The current distance from the object.
 *
 * Return:  The new distance for the object. 
 */
double check_cone_hit(obj_t* obj, double* dir, double* newbase, double t)
{
    cone_t* cone = (cone_t*)(obj->priv);
    double hit[XYZ];
    double scaled_d[XYZ];
    scale3(t, dir, scaled_d);
    sum3(newbase, scaled_d, hit);
    if (hit[Y] < 0 || hit[Y] > cone->height) 
    {
        t = MISS;
    }
    else
    {
        double normal[XYZ];
        double idmat[XYZ][XYZ];
        get_id_matrix(idmat);
        normal[X] = 2 * hit[X];
        normal[Y] = -2 * cone->scale * hit[Y];
        normal[Z] = 2 * hit[Z];
        if (!(cone->centerline[X] == idmat[Y][X] && 
              cone->centerline[Y] != 0 &&
              cone->centerline[Z] == idmat[Y][Z]))
        {
            transform(cone->irot, hit, obj->hitloc);
            transform(cone->irot, normal, obj->normal);
        }
        else
        {
            transform(idmat, hit, obj->hitloc);
            transform(idmat, normal, obj->normal);
        }
        sum3(obj->hitloc, cone->center, obj->hitloc);
    }
    return t;
}

/*
 * Function for dumping the data about cone objects.
 * Param: out  The stream to dump to.
 * Param: obj  The object to dump.
 */
void dump_cone(FILE* out, obj_t* obj)
{
    fprintf(out, "\nCone Data\n");
    cone_t* cone = (cone_t*)obj->priv;
    fprintf(out, "\nCenter %f %f %f\nCenterline: %f %f %f\nRadius: %f\n"
                 "height: %f\n", cone->center[X], cone->center[Y], 
                 cone->center[Z], cone->centerline[X], cone->centerline[Y], 
                 cone->centerline[Z], cone->radius, cone->height);
}

/*
 * Function allowing a hyperboloid to kill itself and free its memory.
 * Param: cone  Cone to kill.
 */
void kill_cone(void* cone)
{
    free(cone);
}
