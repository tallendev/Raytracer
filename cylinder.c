/*
 * Author: Tyler Allen
 * Date: 04/20/2013
 *
 * This file contains logic for creating, destroying, printing, and determining
 * if we hit a cylinder.
 */

/* Header file for this source file. */
#include "cylinder.h"

/*
 * Function for initializing a cylinder.
 * Param: in  The file we are reading in from.
 * Param: objtype  The object type of the object we are creating.
 * Return:  The new object that has been created.
 */
obj_t* cyl_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    int pcount = 0;
    obj_t* obj = object_init(in, objtype);
    if (obj)
    {
        cyl_t* cyl = Malloc(sizeof(cyl_t));
        pcount += fscanf(in, "%lf %lf %lf",
                         &cyl->center[X], &cyl->center[Y], &cyl->center[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf %lf",
                         &cyl->centerline[X], &cyl->centerline[Y], 
                         &cyl->centerline[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf", &cyl->radius, &cyl->height);
        fgets(buff, BUFF_SIZE, in);
        if (pcount == CYL_OBJS)
        {
            double id_matrix[XYZ][XYZ];
            get_id_matrix(id_matrix);
            /* Adjustment for shapes aligned with the negative y axis. This
             * is a minor hack to adjust for this bad case. */
            if ((cyl->centerline[X] == id_matrix[Y][X] &&
                 cyl->centerline[Y] < 0 &&
                 cyl->centerline[Z] == id_matrix[Y][Z]))
            {
                cyl->centerline[X] = ROUNDING_ADJUSTMENT;
            }
            unitvec3(cyl->centerline, cyl->rotmat[Y]);
            cross_prod(cyl->rotmat[Y], id_matrix[Y], cyl->rotmat[X]);
            unitvec3(cyl->rotmat[X], cyl->rotmat[X]);
            cross_prod(cyl->rotmat[X], cyl->rotmat[Y], cyl->rotmat[Z]);
            unitvec3(cyl->rotmat[Z], cyl->rotmat[Z]);
            transpose_mat(cyl->rotmat, cyl->irot);
            obj->hits = cyl_hits;
            obj->priv = cyl;
        }
        else
        {
            fprintf(stderr, "Error found in cyl_init...\n");
            obj = NULL;
        }
    }
    return obj;
}

/*
 * Function for determining if a ray hit a cylinder.
 *
 * Param: base  The starting point for the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The plane object that is being tested for hits.
 *
 * Return: The distance from our viewpoint that the object is at, if we hit.
 *         If we miss, returns -1.
 */
double cyl_hits(double* base, double* dir_start, obj_t* obj)
{
    double t = MISS;
    cyl_t* cyl = (cyl_t*)obj->priv;
    double newbase[XYZ];
    double newcenter[XYZ];
    double dir[XYZ];
    diff3(cyl->center, base, newbase);
    diff3(cyl->center, cyl->center, newcenter);
    double idmat[XYZ][XYZ];
    get_id_matrix(idmat);
    if (!(cyl->centerline[X] == idmat[Y][X] && 
          cyl->centerline[Y] != 0 &&  
          cyl->centerline[Z] == idmat[Y][Z]))
    {
        transform(cyl->rotmat, newbase, newbase);
        transform(cyl->rotmat, dir_start, dir);
    }
    else
    {
        transform(idmat, newbase, newbase);
        transform(idmat, dir_start, dir);
    }
    double a = pow(dir[X], SQUARED) + pow(dir[Z], SQUARED);
    double b = 2 * (newbase[X] * dir[X] + newbase[Z] * dir[Z]);
    double c = pow(newbase[X], SQUARED) + pow(newbase[Z], SQUARED) - 
               pow(cyl->radius, SQUARED);
    double discrim = (pow(b, SQUARED) - 4 * a * c);
    double t1 = ((-1 * b) - sqrt(discrim))/(2 * a);
    double t2 = ((-1 * b) + sqrt(discrim))/(2 * a);
    t2 = (check_cyl_hit (obj, dir, newbase, t2));
    t1 = (check_cyl_hit (obj, dir, newbase, t1));
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
 * return:  The new t value, representing the distance from the object.
 */
double check_cyl_hit(obj_t* obj, double* dir, double* newbase, double t)
{
    cyl_t* cyl = (cyl_t*)(obj->priv);
    double hit[XYZ];
    double scaled_d[XYZ];
    scale3(t, dir, scaled_d);
    sum3(newbase, scaled_d, hit);
    if (hit[Y] < 0 || hit[Y] > cyl->height) 
    {
        t = MISS;
    }
    else
    {
        double normal[XYZ];
        double idmat[XYZ][XYZ];
        get_id_matrix(idmat);
        copy3(hit, normal);
        normal[Y] = 0;
        unitvec3(normal, normal);
        if (!(cyl->centerline[X] == idmat[Y][X] && 
              cyl->centerline[Y] != 0 &&
              cyl->centerline[Z] == idmat[Y][Z]))
        {
            transform(cyl->irot, hit, obj->hitloc);
            transform(cyl->irot, normal, obj->normal);
        }
        else
        {
            transform(idmat, hit, obj->hitloc);
            transform(idmat, normal, obj->normal);
        }
        sum3(obj->hitloc, cyl->center, obj->hitloc);
    }
    return t;
}

/*
 * Function for dumping the data about cylinder objects.
 * Param: out  The stream to dump to.
 * Param: obj  The object to dump.
 */
void dump_cyl(FILE* out, obj_t* obj)
{
    fprintf(out, "\nCylinder Data\n");
    cyl_t* cyl = (cyl_t*)obj->priv;
    fprintf(out, "\nCenter %f %f %f\nCenterline: %f %f %f\nRadius: %f\n"
                 "height: %f\n", cyl->center[X], cyl->center[Y], 
                 cyl->center[Z], cyl->centerline[X], cyl->centerline[Y], 
                 cyl->centerline[Z], cyl->radius, cyl->height);
}

/*
 * Function allowing a cylinder to kill itself and free its memory.
 * Param: cylinder  Cylinder to kill.
 */
void kill_cyl(void* cylinder)
{
    free(cylinder);
}
